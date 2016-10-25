/**
 * @file   SafetyPrepareQueue.hpp
 * @brief  SafetyPrepareQueue class prototype.
 * @author zer0
 * @date   2016-08-04
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_SAFETYPREPAREQUEUE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_SAFETYPREPAREQUEUE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Exception.hpp>
#include <libtbag/id/generator/TimeId.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>

#include <mutex>
#include <atomic>
#include <memory>
#include <deque>
#include <map>
#include <type_traits>

//#define ENABLE_SAFETY_PREPARE_QUEUE_DEBUG

#if defined(ENABLE_SAFETY_PREPARE_QUEUE_DEBUG)
# define SAFETY_PREPARE_QUEUE_DEBUG_PREFIX         "[SafetyPrepareQueue] "
# define SAFETY_PREPARE_QUEUE_DEBUG(m)             tDLogD(SAFETY_PREPARE_QUEUE_DEBUG_PREFIX m)
# define SAFETY_PREPARE_QUEUE_DEBUG_FORMAT(m, ...) tDLogDF(SAFETY_PREPARE_QUEUE_DEBUG_PREFIX m, __VA_ARGS__)
#else
# define SAFETY_PREPARE_QUEUE_DEBUG(m)
# define SAFETY_PREPARE_QUEUE_DEBUG_FORMAT(m, ...)
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace container {

/**
 * SafetyPrepareQueue class prototype.
 *
 * @author zer0
 * @date   2016-08-04
 *
 * @remarks
 *  List of Buffers:
 *  - ACTIVE
 *  - READING
 *  - REMOVE
 *  - PREPARE
 *
 *  Life Cycle:
 *  @code
 *      *<---------------------(OR/prepare)<------------------------*
 *      V                                                           ^
 *      *<---(push)--- [PREPARE/USER_WRITE] <----(prepareManual)----* <== (NEW DATA if EMPTY)
 *      |                                                           |
 *   [ACTIVE] --(popManual)-> [READING/USER_READ] --(readEnd)--> [REMOVE]
 *      V                                                           ^
 *      *----------------------->(OR/pop)-------------------------->*
 *  @endcode
 */
template <typename ValueType
        , typename MutexType = std::mutex>
class SafetyPrepareQueue : public Noncopyable
{

public:
    using Key   = id::Id;
    using Value = ValueType;
    using Mutex = MutexType;
    using Guard = std::lock_guard<Mutex>;

public:
    /**
     * Packet class prototype.
     *
     * @author zer0
     * @date   2016-08-04
     */
    class Packet
    {
    public:
        friend SafetyPrepareQueue;

    protected:
        Key   _id;
        Value _value;

    public:
        Packet() : _id(id::generator::genTimeId()), _value()
        { /* EMPTY. */ }
        ~Packet()
        { /* EMPTY. */ }

        Packet(Packet const & obj) = default;
        Packet & operator =(Packet const & obj) = default;

#if defined(TBAG_HAS_DEFAULTED_FUNCTIONS) && !defined(TBAG_HAS_DEFAULTED_FUNCTIONS_BUT_NOT_MOVE_FUNCTION)
        Packet(Packet && obj) = default;
        Packet & operator =(Packet && obj) = default;
#endif

    public:
        inline Key getId() const TBAG_NOEXCEPT
        { return _id;    }

    public:
        inline Value const & at() const
        { return _value; }
        inline Value & at()
        { return _value; }
    };

    /**
     * PreparePacket class prototype.
     *
     * @author zer0
     * @date   2016-08-05
     */
    struct PreparePacket : public Noncopyable
    {
    private:
        SafetyPrepareQueue & _queue;
        Packet & _packet;
        std::atomic_bool _cancel;

    public:
        PreparePacket(SafetyPrepareQueue & queue, Packet & packet)
                : _queue(queue), _packet(packet), _cancel(false)
        {
            // EMPTY.
        }

        ~PreparePacket()
        {
            if (_cancel == false) {
                _queue.push(_packet);
            }
        }

    public:
        void cancel()
        {
            bool compare = false;
            if (_cancel.compare_exchange_weak(compare, true)) {
                _queue.cancelPrepare(_packet);
            }
        }

        inline Value & at() throw (IllegalStateException)
        {
            if (_cancel == true) {
                throw IllegalStateException();
            }
            return _packet.at();
        }
    };

    /**
     * ReadablePacket class prototype.
     *
     * @author zer0
     * @date   2016-08-05
     */
    struct ReadablePacket : public Noncopyable
    {
    private:
        SafetyPrepareQueue & _queue;
        Packet const & _packet;
        std::atomic_bool _cancel;

    public:
        ReadablePacket(SafetyPrepareQueue & queue, Packet const & packet)
                : _queue(queue), _packet(packet), _cancel(false)
        {
            // EMPTY.
        }

        ~ReadablePacket()
        {
            if (_cancel == false) {
                _queue.readEnd(_packet);
            }
        }

    public:
        void cancel()
        {
            bool compare = false;
            if (_cancel.compare_exchange_weak(compare, true)) {
                _queue.cancelPop(_packet);
            }
        }

        inline Value const & at() const throw (IllegalStateException)
        {
            if (_cancel == true) {
                throw IllegalStateException();
            }
            return _packet.at();
        }
    };

    using Prepare  = std::shared_ptr<PreparePacket>;
    using Readable = std::shared_ptr<ReadablePacket>;

    using Queue         = std::deque<Packet>;
    using PacketMap     = std::map<Key, Packet>;
    using PacketMapItr  = typename PacketMap::iterator;
    using PacketMapPair = typename PacketMap::value_type;

private:
    mutable Mutex _mutex;
    Queue     _active_queue;
    PacketMap _reading_map;
    PacketMap _remove_map;
    PacketMap _prepare_map;

public:
    SafetyPrepareQueue()
    {
        // EMPTY.
    }

    ~SafetyPrepareQueue()
    {
        clear();
    }

public:
    void clear()
    {
        Guard guard(_mutex);
        while (_active_queue.empty() == false) {
            _active_queue.pop_front();
        }
        _reading_map.clear();
        _remove_map.clear();
        _prepare_map.clear();
    }

public:
    Packet & prepareManual()
    {
        Guard guard(_mutex);

        auto itr = _remove_map.begin();
        if (itr != _remove_map.end()) {
            auto inserted_packet = _prepare_map.insert(PacketMapPair(itr->first, itr->second));
            assert(inserted_packet.second);
            _remove_map.erase(itr);
            return inserted_packet.first->second;
        }

        Packet new_packet;
        return _prepare_map.insert(PacketMapPair(new_packet._id, new_packet)).first->second;
    }

    inline Prepare prepare()
    {
        return Prepare(new PreparePacket(*this, prepareManual()));
    }

    void cancelPrepare(Packet const & packet) throw (NotFoundException)
    {
        Guard guard(_mutex);

        auto itr = _prepare_map.find(packet.getId());
        if (itr == _prepare_map.end()) {
            throw NotFoundException();
        }

        auto inserted_packet = _remove_map.insert(PacketMapPair(itr->first, itr->second));
        assert(inserted_packet.second);
        _prepare_map.erase(itr);
    }

    void push(Packet const & packet) throw (NotFoundException)
    {
        Guard guard(_mutex);
        auto itr = _prepare_map.find(packet.getId());
        if (itr == _prepare_map.end()) {
            throw NotFoundException();
        }

        _active_queue.push_back(itr->second);
        _prepare_map.erase(itr);

        SAFETY_PREPARE_QUEUE_DEBUG_FORMAT("PUSH (a{}/r{}/m{}/p{})"
                , _active_queue.size()
                , _reading_map.size()
                , _remove_map.size()
                , _prepare_map.size());
    }

    Packet const & popManual() throw (ContainerEmptyException)
    {
        Guard guard(_mutex);
        if (_active_queue.empty()) {
            throw ContainerEmptyException();
        }

        auto packet = _active_queue.front();
        _active_queue.pop_front();
        Packet const & result = _reading_map.insert(PacketMapPair(packet._id, packet)).first->second;

        SAFETY_PREPARE_QUEUE_DEBUG_FORMAT("POP (a{}/r{}/m{}/p{})"
                , _active_queue.size()
                , _reading_map.size()
                , _remove_map.size()
                , _prepare_map.size());

        return result;
    }

    inline Readable pop() throw (ContainerEmptyException)
    {
        return Readable(new ReadablePacket(*this, popManual()));
    }

    void cancelPop(Packet const & packet) throw (NotFoundException)
    {
        Guard guard(_mutex);

        auto itr = _reading_map.find(packet.getId());
        if (itr == _reading_map.end()) {
            throw NotFoundException();
        }

        _active_queue.push_front(itr->second);
        _reading_map.erase(itr);
    }

    bool popAndReadEnd() throw (IllegalArgumentException)
    {
        Guard guard(_mutex);
        auto packet = _active_queue.front();
        _active_queue.pop_front();
        return _remove_map.insert(PacketMapPair(packet._id, packet)).second;
    }

    Readable popUntil(std::size_t size = 0U) throw (IllegalStateException)
    {
        Guard guard(_mutex);
        if (_active_queue.size() <= size) {
            throw IllegalStateException();
        }

        while (_active_queue.size() - 1 > size) {
            auto packet = _active_queue.front();
            _active_queue.pop_front();
            _remove_map.insert(PacketMapPair(packet._id, packet));
        }

        auto packet = _active_queue.front();
        _active_queue.pop_front();
        return Readable(new ReadablePacket(*this, _reading_map.insert(PacketMapPair(packet._id, packet)).first->second));
    }

    void readEnd(Packet const & packet) throw (NotFoundException)
    {
        Guard guard(_mutex);
        auto itr = _reading_map.find(packet.getId());
        if (itr == _reading_map.end()) {
            throw NotFoundException();
        }

        _remove_map.insert(PacketMapPair(itr->first, itr->second));
        _reading_map.erase(itr);
    }

public:
    inline std::size_t size() const TBAG_NOEXCEPT
    { Guard g(_mutex); return _active_queue.size(); }
    inline std::size_t sizeOfReading() const TBAG_NOEXCEPT
    { Guard g(_mutex); return _reading_map.size();  }
    inline std::size_t sizeOfRemove() const TBAG_NOEXCEPT
    { Guard g(_mutex); return _remove_map.size();   }
    inline std::size_t sizeOfPrepare() const TBAG_NOEXCEPT
    { Guard g(_mutex); return _prepare_map.size();   }

    inline std::size_t sizeOfAll() const TBAG_NOEXCEPT
    {
        Guard g(_mutex);
        return   _active_queue.size()
               +  _reading_map.size()
               +   _remove_map.size()
               +  _prepare_map.size();
    }

    inline bool empty() const TBAG_NOEXCEPT
    { Guard g(_mutex); return _active_queue.empty(); }
    inline bool emptyOfReading() const TBAG_NOEXCEPT
    { Guard g(_mutex); return _reading_map.empty();  }
    inline bool emptyOfRemove() const TBAG_NOEXCEPT
    { Guard g(_mutex); return _remove_map.empty();   }
    inline bool emptyOfPrepare() const TBAG_NOEXCEPT
    { Guard g(_mutex); return _prepare_map.empty();   }
};

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_SAFETYPREPAREQUEUE_HPP__

