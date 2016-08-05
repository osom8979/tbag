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
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Exception.hpp>
#include <libtbag/id/generator/TimeId.hpp>

#include <mutex>
#include <memory>
#include <queue>
#include <map>
#include <type_traits>

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
 *  @code
 *      *<---------------(OR/autoPrepare)<--------------------*
 *      *<---(push)--- [USER_WRITE/PREPARE] <----(prepare)----*
 *      |                                                     |
 *   [ACTIVE] --(pop)-> [READING/USER_READ] --(readEnd)--> [REMOVE] <== (NEW DATA if EMPTY)
 *      *----------------->(OR/autoPop)---------------------->*
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
    enum class PacketState : int
    {
        ACTIVE,
        READING,
        REMOVE,
        PREPARE,
    };

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
        PacketState  _state;
        Key          _id;
        Value        _value;

    public:
        Packet() : _state(PacketState::PREPARE), _id(id::generator::genTimeId()), _value()
        {
            // EMPTY.
        }

        Packet(Packet const & obj) = default;
        Packet(Packet && obj) = default;

        ~Packet() = default;

        Packet & operator =(Packet const & obj) = default;
        Packet & operator =(Packet && obj) = default;

    public:
        inline PacketState getState() const noexcept
        { return _state; }
        inline Key getId() const noexcept
        { return _id;    }

    public:
        inline Value const & at() const throw (IllegalStateException)
        {
            if (_state != PacketState::READING) {
                throw IllegalStateException();
            }
            return _value;
        }

        inline Value & at() throw (IllegalStateException)
        {
            if (_state != PacketState::PREPARE) {
                throw IllegalStateException();
            }
            return _value;
        }
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

    public:
        PreparePacket(SafetyPrepareQueue & queue, Packet & packet)
                : _queue(queue), _packet(packet)
        {
            // EMPTY.
        }

        ~PreparePacket()
        {
            _queue.push(_packet);
        }

    public:
        inline Value & at()
        { return _packet.at(); }
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

    public:
        ReadablePacket(SafetyPrepareQueue & queue, Packet const & packet)
                : _queue(queue), _packet(packet)
        {
            // EMPTY.
        }

        ~ReadablePacket()
        {
            _queue.readEnd(_packet);
        }

    public:
        inline Value const & at() const
        { return _packet.at(); }
    };

    using Prepare  = std::shared_ptr<PreparePacket>;
    using Readable = std::shared_ptr<ReadablePacket>;

    using Queue         = std::queue<Packet>;
    using PacketMap     = std::map<Key, Packet>;
    using PacketMapPair = typename PacketMap::value_type;

private:
    mutable Mutex _mutex;
    Queue         _active_queue;
    PacketMap     _reading_map;
    PacketMap     _remove_map;

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
            _active_queue.pop();
        }
        _reading_map.clear();
        _remove_map.clear();
    }

private:
    static inline bool compareExchangeState(PacketState & state, PacketState expected, PacketState desired) noexcept
    {
        if (state == expected) {
            state = desired;
            return true;
        }
        return false;
    }

    static inline bool tryActiveToReading(PacketState & state) noexcept
    { return compareExchangeState(state, PacketState::ACTIVE, PacketState::READING); }
    static inline bool tryReadingToRemove(PacketState & state) noexcept
    { return compareExchangeState(state, PacketState::READING, PacketState::REMOVE); }
    static inline bool tryRemoveToPrepare(PacketState & state) noexcept
    { return compareExchangeState(state, PacketState::REMOVE, PacketState::PREPARE); }
    static inline bool tryPrepareToActive(PacketState & state) noexcept
    { return compareExchangeState(state, PacketState::PREPARE, PacketState::ACTIVE); }

public:
    Packet & prepare()
    {
        Guard guard(_mutex);

        auto itr = _remove_map.begin();
        auto end = _remove_map.end();

        static_assert(std::is_const<decltype(itr)>::value == false
                , "Iterator is should not be const-type.");

        for (; itr != end; ++itr) {
            // FIND READY DATA.
            if (tryRemoveToPrepare(itr->second._state)) {
                return itr->second;
            }
        }

        Packet new_packet;
        return _remove_map.insert(PacketMapPair(new_packet._id, new_packet)).first->second;
    }

    inline Prepare autoPrepare()
    {
        return Prepare(new PreparePacket(*this, prepare()));
    }

    void push(Packet const & packet) throw (IllegalArgumentException, NotFoundException)
    {
        Guard guard(_mutex);
        auto itr = _remove_map.find(packet.getId());
        if (itr == _remove_map.end()) {
            throw NotFoundException();
        }

        if (tryPrepareToActive(itr->second._state) == false) {
            throw IllegalArgumentException();
        }

        _active_queue.push(itr->second);
        _remove_map.erase(itr);
    }

    Packet const & pop() throw (ContainerEmptyException, IllegalArgumentException)
    {
        Guard guard(_mutex);
        if (_active_queue.empty()) {
            throw ContainerEmptyException();
        }

        auto packet = _active_queue.front();
        if (tryActiveToReading(packet._state) == false) {
            throw IllegalArgumentException();
        }

        _active_queue.pop();
        return _reading_map.insert(PacketMapPair(packet._id, packet)).first->second;
    }

    inline Readable autoPop() throw (ContainerEmptyException, IllegalArgumentException)
    {
        return Readable(new ReadablePacket(*this, pop()));
    }

    bool popAndReadEnd() throw (IllegalArgumentException)
    {
        Guard guard(_mutex);
        auto packet = _active_queue.front();
        if (tryActiveToReading(packet._state) == false) {
            throw IllegalArgumentException();
        }

        _active_queue.pop();
        return _remove_map.insert(PacketMapPair(packet._id, packet)).second;
    }

    void readEnd(Packet const & packet) throw (IllegalArgumentException, NotFoundException)
    {
        Guard guard(_mutex);
        auto itr = _reading_map.find(packet.getId());
        if (itr == _reading_map.end()) {
            throw NotFoundException();
        }

        if (tryReadingToRemove(itr->second._state) == false) {
            throw IllegalArgumentException();
        }

        _remove_map.insert(PacketMapPair(itr->first, itr->second));
        _reading_map.erase(itr);
    }

public:
    inline std::size_t size() const noexcept
    { return _active_queue.size(); }
    inline std::size_t sizeOfReadingSet() const noexcept
    { return _reading_map.size();  }
    inline std::size_t sizeOfRemoveSet() const noexcept
    { return _remove_map.size();   }

    inline bool empty() const noexcept
    { return _active_queue.empty(); }
    inline bool emptyOfReadingSet() const noexcept
    { return _reading_map.empty();  }
    inline bool emptyOfRemoveSet() const noexcept
    { return _remove_map.empty();   }
};

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_SAFETYPREPAREQUEUE_HPP__

