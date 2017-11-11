/**
 * @file   EnttTest.cpp
 * @brief  Entt library tester.
 * @author zer0
 * @date   2017-11-11
 */

#include <gtest/gtest.h>
#include <libtbag/3rd/entt/entt.hpp>

namespace __impl {

struct Position
{
    int x;
    int y;
};

struct Velocity
{
    int dx;
    int dy;
};

} // namespace __impl

TEST(EnttTest, Default)
{
    using namespace __impl;
    entt::DefaultRegistry registry;

    ASSERT_TRUE(registry.empty());
    ASSERT_EQ(0, registry.size());

    auto empty_entity = registry.create();
    ASSERT_TRUE(registry.valid(empty_entity));
    ASSERT_FALSE(registry.empty());
    ASSERT_EQ(1, registry.size());

    registry.destroy(empty_entity);
    ASSERT_FALSE(registry.valid(empty_entity));
    ASSERT_TRUE(registry.empty());
    ASSERT_EQ(0, registry.size());

    auto entity = registry.create(Position{1, 2});
    ASSERT_TRUE(registry.valid(entity));
    ASSERT_EQ(1, registry.size());

    bool has_result = false;
    ASSERT_TRUE( registry.has<Position>(entity));
    ASSERT_FALSE(registry.has<Velocity>(entity));
    has_result = registry.has<Position, Velocity>(entity);
    ASSERT_FALSE(has_result);

    auto & entity_velocity = registry.assign<Velocity>(entity, 11, 12);
    ASSERT_EQ(11, entity_velocity.dx);
    ASSERT_EQ(12, entity_velocity.dy);

    entity_velocity.dx = 21;
    entity_velocity.dy = 22;

    ASSERT_TRUE(registry.has<Position>(entity));
    ASSERT_TRUE(registry.has<Velocity>(entity));
    has_result = registry.has<Position, Velocity>(entity);
    ASSERT_TRUE(has_result);

    ASSERT_EQ( 1, registry.get<Position>(entity).x);
    ASSERT_EQ( 2, registry.get<Position>(entity).y);
    ASSERT_EQ(21, registry.get<Velocity>(entity).dx);
    ASSERT_EQ(22, registry.get<Velocity>(entity).dy);

    // registry.assign<Velocity>(entity, 31, 32); // Don't use this method: Assertion failed: (!has(entity))
    registry.accomodate<Velocity>(entity, 31, 32);

    ASSERT_EQ(31, registry.get<Velocity>(entity).dx);
    ASSERT_EQ(32, registry.get<Velocity>(entity).dy);

    auto entity2 = registry.create<Position>();
    ASSERT_TRUE( registry.has<Position>(entity2));
    ASSERT_FALSE(registry.has<Velocity>(entity2));
    ASSERT_EQ(2, registry.size());

    registry.replace<Position>(entity2, 0, 0);

    auto view = registry.view<Position, Velocity>();
    for (auto e : view) {
        auto & pos = view.get<Position>(e);
        auto & vel = view.get<Velocity>(e);
        pos.x = 101;
        pos.y = 102;
    }

    ASSERT_EQ(101, registry.get<Position>(entity).x);
    ASSERT_EQ(102, registry.get<Position>(entity).y);
    ASSERT_EQ( 31, registry.get<Velocity>(entity).dx);
    ASSERT_EQ( 32, registry.get<Velocity>(entity).dy);
    ASSERT_EQ(  0, registry.get<Position>(entity2).x);
    ASSERT_EQ(  0, registry.get<Position>(entity2).y);

    auto view2 = registry.view<Position>();
    for (auto /* Don't use '&' & */ e : view2) {
        auto & pos = view2.get(e);
        pos.x = 201;
        pos.y = 202;
    }

    ASSERT_EQ(201, registry.get<Position>(entity).x);
    ASSERT_EQ(202, registry.get<Position>(entity).y);
    ASSERT_EQ( 31, registry.get<Velocity>(entity).dx);
    ASSERT_EQ( 32, registry.get<Velocity>(entity).dy);
    ASSERT_EQ(201, registry.get<Position>(entity2).x);
    ASSERT_EQ(202, registry.get<Position>(entity2).y);

    registry.view<Position, Velocity>().each([](auto entity, auto & position, auto & velocity) {
        position.x  = 0;
        position.y  = 0;
        velocity.dx = 0;
        velocity.dy = 0;
    });

    ASSERT_EQ(  0, registry.get<Position>(entity).x);
    ASSERT_EQ(  0, registry.get<Position>(entity).y);
    ASSERT_EQ(  0, registry.get<Velocity>(entity).dx);
    ASSERT_EQ(  0, registry.get<Velocity>(entity).dy);
    ASSERT_EQ(201, registry.get<Position>(entity2).x);
    ASSERT_EQ(202, registry.get<Position>(entity2).y);
}

