#pragma once
#include <windows.h>

constexpr std::uintptr_t UWORLD = 0x178685D8;//
constexpr std::uintptr_t GNAMES = 0x17722E80;//
constexpr std::uintptr_t ACKNOWLEDGED_PAWN = 0x358;//
constexpr std::uintptr_t PLAYER_CONTROLLER = 0x30;//
constexpr std::uintptr_t LOCAL_PLAYERS = 0x38;//
constexpr std::uintptr_t PLAYER_STATE = 0x2D0;//
constexpr std::uintptr_t GAME_INSTANCE = 0x248; //
constexpr std::uintptr_t GAME_STATE = 0x1D0; //
constexpr std::uintptr_t PLAYER_ARRAY = 0x2C8; //
constexpr std::uintptr_t ROOT_COMPONENT = 0x1B0; //
constexpr std::uintptr_t CURRENT_VEHICLE = 0x2C68; //
constexpr std::uintptr_t RELATIVE_LOCATION = 0x140; //
constexpr std::uintptr_t RELATIVE_ROTATION = 0x158;//
constexpr std::uintptr_t PAWN_PRIVATE = 0x328; //
constexpr std::uintptr_t MESH = 0x330; //
constexpr std::uintptr_t TEAM_INDEX = 0x11B1; //
constexpr std::uintptr_t B_IS_DYING = 0x728; //
constexpr std::uintptr_t B_IS_DBNO = 0x841;//
constexpr std::uintptr_t BONE_ARRAY = 0x5F0; //
constexpr std::uintptr_t COMPONENT_TO_WORLD = 0x1E0; //
constexpr std::uintptr_t LAST_SUBMIT_TIME_ON_SCREEN = 0x328; //GETTHSI!!!
constexpr std::uintptr_t CURRENT_WEAPON = 0x990; //
constexpr std::uintptr_t WEAPON_DATA = 0x5A0;  //
constexpr std::uintptr_t AMMO_COUNT = 0x14FC;//
constexpr std::uintptr_t ITEM_NAME = 0xB0; //
constexpr std::uintptr_t PRIMARY_PICKUP_ITEM_ENTRY = 0x3A8; //
constexpr std::uintptr_t TIER = 0xAA;
constexpr std::uintptr_t COLOR = 0xA2;
constexpr std::uintptr_t HABANERO_COMPONENT = 0x948;//
constexpr std::uintptr_t USERNAME_COMPONENT = 0xA08; //0xA08
constexpr std::uintptr_t PLATFORM = 0x440; //
constexpr std::uintptr_t TARGETED_FORT_PAWN = 0x1830; //
constexpr std::uintptr_t KILL_SCORE = 0x11C8; //
constexpr std::uintptr_t RANKED_PROGRESS = 0xD8; //
constexpr std::uintptr_t CAMERA_LOCATION = 0x170; //
constexpr std::uintptr_t CAMERA_ROTATION = 0x180;//
constexpr std::uintptr_t PLAYER_FOV = 0x3B4; // camerafov
constexpr std::uintptr_t WORLD_TIME_SECONDS = 0x190; //seconds 
constexpr std::uintptr_t REBOOTS = 0x1894; //
constexpr std::uintptr_t ACTORS = 0x48; //WHAT IS THIS 
constexpr std::uintptr_t LEVELS = 0x1E8;//
constexpr std::uintptr_t ITSBOT = 0x2BA; //
constexpr std::uintptr_t PROJECTILE_SPEED = 0x24AC; //
constexpr std::uintptr_t PROJECTILE_GRAVITY = 0x24B0; //
constexpr std::uintptr_t CAMERA_MANAGER = 0x360; //
constexpr std::uintptr_t PLAYERCAMERAMANAGER = 0x360;
constexpr std::uintptr_t CURRENT_MOVEMENT_STYLE = 0x843;
constexpr std::uintptr_t DEFAULTFOV = 0x2C4;
constexpr std::uintptr_t BASEFOV = 0x710;
constexpr std::uintptr_t Spectators = 0xA98;
constexpr std::uintptr_t bIsReloadingWeapon = 0x3A1; //
constexpr std::uintptr_t NetConnection = 0x528;
constexpr std::uintptr_t RotationInput = NetConnection + 0x8;



template< typename t >
class TArray
{
public:
    TArray() : tData(), iCount(), iMaxCount() {}
    TArray(t* data, int count, int max_count) :
        tData(tData), iCount(iCount), iMaxCount(iMaxCount) {
    }

public:
    auto Get(int idx) -> t
    {
        return kernel->read_t< t >(reinterpret_cast<__int64>(this->tData) + (idx * sizeof(t)));
    }

    auto Size() -> std::uint32_t
    {
        return this->iCount;
    }

    bool IsValid()
    {
        return this->iCount != 0;
    }

    t* tData;
    int iCount;
    int iMaxCount;
    std::uintptr_t Array;
    std::uint32_t Count;
    std::uint32_t MaxCount;

    t Get(std::uint32_t Index) const {
        if (Index >= Count) {
            return t();
        }
        return kernel->read_t<t>(Array + (Index * sizeof(t)));
    }

    t operator[](std::uint32_t Index) const {
        return Get(Index);
    }

    std::uint32_t size() const {
        return Count;
    }

    bool isValid() const {
        return Array && Count <= MaxCount && MaxCount <= 1000000;
    }

    std::uintptr_t getAddress() const {
        return Array;
    }
};
template<class T>
class FArray
{
public:
    int getLength() const
    {
        return count;
    }

    int getIdentifier()
    {
        return data * count * max;
    }

    bool isValid() const
    {
        if (count > max)
            return false;
        if (!data)
            return false;
        return true;
    }

    uint64_t getAddress() const
    {
        return data;
    }

    T operator [](size_t idx) const
    {
        return kernel->read_t<T>(data + sizeof(T) * idx);
    }

protected:
    uint64_t data;
    uint64_t count;
    uint64_t max;
};
