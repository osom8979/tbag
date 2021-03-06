/**
 * @file   FileKeyStore.cpp
 * @brief  FileKeyStore class implementation.
 * @author zer0
 * @date   2017-12-07
 */

#include <libtbag/security/store/FileKeyStore.hpp>
#include <libtbag/string/Format.hpp>
#include <libtbag/crypto/PseudoRandom.hpp>
#include <libtbag/crypto/Pbkdf2.hpp>

#include <cassert>
#include <tuple>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace security {
namespace store    {

// ---------------
namespace __impl {
// ---------------

static std::string getCreateTableQuery()
{
    using namespace libtbag::string;
    return fformat("CREATE TABLE IF NOT EXISTS {} ({} TEXT PRIMARY KEY, {} TEXT, {} TEXT);",
                   FileKeyStore::getTableName(), FileKeyStore::getKeyName(),
                   FileKeyStore::getSaltName(), FileKeyStore::getValueName());
}

static std::string getInsertQuery(std::string const & key,
                                  std::string const & salt,
                                  std::string const & value)
{
    using namespace libtbag::string;
    return fformat("INSERT INTO {}({}, {}, {}) VALUES('{}', '{}', '{}');",
                   FileKeyStore::getTableName(), FileKeyStore::getKeyName(),
                   FileKeyStore::getSaltName(), FileKeyStore::getValueName(),
                   key, salt, value);
}

static std::string getUpdateQuery(std::string const & key,
                                  std::string const & salt,
                                  std::string const & value)
{
    using namespace libtbag::string;
    return fformat("UPDATE {} SET {}='{}', {}='{}' WHERE {} LIKE '{}';",
                   FileKeyStore::getTableName(),
                   FileKeyStore::getSaltName(), salt,
                   FileKeyStore::getValueName(), value,
                   FileKeyStore::getKeyName(), key);
}

static std::string getSelectQuery(std::string const & key)
{
    using namespace libtbag::string;
    return fformat("SELECT {}, {} FROM {} WHERE {}='{}';",
                   FileKeyStore::getSaltName(), FileKeyStore::getValueName(),
                   FileKeyStore::getTableName(), FileKeyStore::getKeyName(), key);
}

static std::string getExistsSelectQuery(std::string const & key)
{
    using namespace libtbag::string;
    return fformat("SELECT {} FROM {} WHERE {}='{}';",
                   FileKeyStore::getKeyName(), FileKeyStore::getTableName(),
                   FileKeyStore::getKeyName(), key);
}

static std::string getSelectKeysQuery()
{
    using namespace libtbag::string;
    return fformat("SELECT {} FROM {};", FileKeyStore::getKeyName(), FileKeyStore::getTableName());
}

static std::string getDeleteQuery(std::string const & key)
{
    using namespace libtbag::string;
    return fformat("DELETE FROM {} WHERE {}='{}';",
                   FileKeyStore::getTableName(), FileKeyStore::getKeyName(), key);
}

static bool exists(FileKeyStore::Sqlite & db, std::string const & key)
{
    using Row = std::tuple<std::string>;
    using Statement = FileKeyStore::Sqlite::Statement;

    auto rows = db.prepare<Row>(getExistsSelectQuery(key), [](Statement const & statement) -> Row {
        return Row(statement.getString(0));
    });
    if (rows.empty()) {
        return false;
    }
    assert(rows.size() == 1u);
    assert(std::get<0>(rows[0]) == key);
    return true;
}

static bool get(FileKeyStore::Sqlite & db, std::string const & key, std::string & salt, std::string & value)
{
    using Row = std::tuple<std::string, std::string>;
    using Statement = FileKeyStore::Sqlite::Statement;

    auto rows = db.prepare<Row>(getSelectQuery(key), [](Statement const & statement) -> Row {
        return Row(statement.getString(0), statement.getString(1));
    });
    if (rows.empty()) {
        return false;
    }

    assert(rows.size() == 1u);
    salt = std::get<0>(rows[0]);
    value = std::get<1>(rows[0]);
    return true;
}

// ------------------
} // namespace __impl
// ------------------

FileKeyStore::FileKeyStore(std::string const & path)
{
    Guard const G(_mutex);
    if (!_db.open(path)) {
        throw std::bad_alloc();
    }
    if (!_db.execute(__impl::getCreateTableQuery())) {
        throw std::bad_alloc();
    }
}

FileKeyStore::~FileKeyStore()
{
    Guard const G(_mutex);
    _db.close();
}

bool FileKeyStore::create(std::string const & key)
{
    Guard const G(_mutex);
    return _db.execute(__impl::getInsertQuery(key, std::string(), std::string()));
}

bool FileKeyStore::remove(std::string const & key)
{
    Guard const G(_mutex);
    return _db.execute(__impl::getDeleteQuery(key));
}

bool FileKeyStore::exists(std::string const & key) const
{
    Guard const G(_mutex);
    return __impl::exists(_db, key);
}

bool FileKeyStore::get(std::string const & key, std::string & result) const
{
    Guard const G(_mutex);
    std::string salt;
    return __impl::get(_db, key, salt, result);
}

bool FileKeyStore::set(std::string const & key, std::string const & value, bool encrypt)
{
    Guard const G(_mutex);
    std::string salt;
    std::string update_value;
    if (encrypt) {
        salt = crypto::generatePseudoRandomString(getSaltSize());
        update_value = crypto::generatePbkdf2String(value, salt, getHashSize());
    } else {
        update_value = value;
    }
    return _db.execute(__impl::getUpdateQuery(key, salt, update_value));
}

bool FileKeyStore::cmp(std::string const & key, std::string const & value, bool encrypt) const
{
    Guard const G(_mutex);
    std::string read_salt;
    std::string read_value;
    if (!__impl::get(_db, key, read_salt, read_value)) {
        return false;
    }

    if (encrypt) {
        return crypto::generatePbkdf2String(value, read_salt, getHashSize()) == read_value;
    } else {
        return value == read_value;
    }
}

std::set<std::string> FileKeyStore::list() const
{
    Guard const G(_mutex);

    using Row = std::tuple<std::string>;
    using Statement = FileKeyStore::Sqlite::Statement;

    auto rows = _db.prepare<Row>(__impl::getSelectKeysQuery(), [](Statement const & statement) -> Row {
        return Row(statement.getString(0));
    });

    if (rows.empty()) {
        return {};
    }

    std::set<std::string> result;
    for (auto const & r : rows) {
        result.emplace(std::get<0>(r));
    }
    return result;
}

} // namespace store
} // namespace security

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

