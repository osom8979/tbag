/**
 * @file   SqliteOrmTest.cpp
 * @brief  SqliteOrm library tester.
 * @author zer0
 * @date   2017-11-19
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/3rd/sqlite_orm/sqlite_orm.h>

namespace __impl {

struct Employee
{
    int id;
    std::string name;
    int age;
    std::string address;
    double salary;
};

} // namespace __impl

using namespace sqlite_orm;
using namespace __impl;

TEST(SqliteOrmTest, Insert)
{
    tttDir(true, true);
    auto const PATH = tttDir_Get();

    auto stor = make_storage((PATH / "test.db").toString(),
                             make_table("COMPANY",
                                        make_column(     "ID", &Employee::id, primary_key()),
                                        make_column(   "NAME", &Employee::name),
                                        make_column(    "AGE", &Employee::age),
                                        make_column("ADDRESS", &Employee::address),
                                        make_column( "SALARY", &Employee::salary)));
    stor.sync_schema();
    stor.remove_all<Employee>();

    // insert initial values.
    stor.replace(Employee{1,  "Paul", 32, "California", 20000.0});
    stor.replace(Employee{2, "Allen", 25,      "Texas", 15000.0});
    stor.replace(Employee{3, "Teddy", 23,     "Norway", 20000.0});
    stor.replace(Employee{4,  "Mark", 25,  "Rich-Mond", 65000.0});
    stor.replace(Employee{5, "David", 27,      "Texas", 85000.0});
    stor.replace(Employee{6,   "Kim", 22, "South-Hall", 45000.0});
    stor.replace(Employee{7, "James", 24,    "Houston", 10000.0});

    // show 'COMPANY' table contents.
    for (auto & employee : stor.iterate<Employee>()) {
        std::cout << stor.dump(employee) << std::endl;
    }
    std::cout << std::endl;

    // 'UPDATE COMPANY SET ADDRESS = 'Texas' WHERE ID = 6'

    auto employee6 = stor.get<Employee>(6);
    employee6.address = "Texas";

    // actually this call updates all non-primary-key columns' values to passed object's fields
    stor.update(employee6);

    // show 'COMPANY' table contents again
    for (auto & employee : stor.iterate<Employee>()) {
        std::cout << stor.dump(employee) << std::endl;
    }
    std::cout << std::endl;

    // 'UPDATE COMPANY SET ADDRESS = 'Texas', SALARY = 20000.00 WHERE AGE < 30'
    stor.update_all(set(&Employee::address, "Texas",
                        &Employee::salary, 20000.00),
                    where(lesser_than(&Employee::age, 30)));

    // show 'COMPANY' table contents one more time
    for (auto & employee : stor.iterate<Employee>()) {
        std::cout << stor.dump(employee) << std::endl;
    }
    std::cout << std::endl;
}

