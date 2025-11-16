cmake_minimum_required(VERSION 3.26)
project(LAB_2)

set(CMAKE_CXX_STANDARD 14)

add_executable(LAB_2
        main.cpp
        AbstractFactory.h
        FactoryMethod.h
        Builder+Prototype.h
        Singleton.h
        CraftableWeapon.h)
