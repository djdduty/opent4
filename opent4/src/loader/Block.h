#ifndef LOADER_BLOCK_H
#define LOADER_BLOCK_H

#include <string>
#include <vector>

#include "loader/Util.h"

namespace opent4
{
    enum BLOCK_TYPE
    {
        //ATI root block IDs
        BT_ACTOR,
        BT_PATH,
        BT_NAVDATA,

        //Actor properties
        BT_ACTOR_ID,
        BT_ACTOR_POSITION,
        BT_ACTOR_ROTATION,
        BT_ACTOR_SCALE,
        BT_ACTOR_NAME,
        BT_ACTOR_VARIABLES,
        BT_ACTOR_LINK,
        BT_ACTOR_CAUSE,
        BT_ACTOR_EVENT,
        BT_ACTOR_PATH_ID,
        BT_ACTOR_CODE,
        BT_ACTOR_MESH_AXIS,
        BT_ACTOR_MESH_BOUNDS,
        BT_ACTOR_PROPERTIES,
        BT_LINK_LISTS,
        BT_ACTOR_TEXTURE_SET,
        BT_GRND_Y,
        BT_MODES,

        //Actor data
        BT_ACTOR_MESH,
        BT_ACTOR_INSTANCES,
        BT_ACTOR_PRECACHE_FILE,

        //Actor variables
        BT_AFFECTS_TARGET,
        BT_TIME_TO_REACH_MAX_STRENGTH,
        BT_TIME_TO_TURN_OFF,
        BT_HIDE,
        BT_HOLD,
        BT_GA_GROUP_NAME,
        BT_GA_MIN_NUMBERS,
        BT_GA_GROUP_ATTACK,
        BT_SXZ_ANGLE,
        BT_SOUND_RADIUS,
        BT_CLOSE_RANGE_DIST,
        BT_MEDIUM_RANGE_DIST,
        BT_AU_A,
        BT_AU_B,
        BT_AU_C,
        BT_AU_D,
        BT_HEALTH,
        BT_MAX_HEALTH,
        BT_TURN_ON,
        BT_TYPE,
        BT_COUNTERS,
        BT_ONLY_P_TARGET,
        BT_SPAWN_ACTOR1_CHANCE,
        BT_CLOSE_RANGE_CHANCE,
        BT_IGNORE_PLAYER,
        BT_PROVOKE_ONLY,
        BT_FD_MULT,
        BT_COLLIDES,
        BT_IGNORES,
        BT_TOUCHES,
        BT_LIGHT_COLOR,
        BT_LIGHT_INTENSITY,
        BT_FRICTION,
        BT_SPIN_X,
        BT_SPIN_Y,
        BT_SPIN_Z,
        BT_A_NDD,
        BT_A_MDD,
        BT_A_SON,
        BT_A_FL,
        BT_C_B,
        BT_F_B,
        BT_F_C,
        BT_F_F,
        BT_F_T,
        BT_F_ED,
        BT_SM_TYPE,
        BT_LEASH_RADIUS,
        BT_LD_SOUND,
        BT_MD_SOUND,
        BT_HD_SOUND,
        BT_SIGHT_RADIUS,
        BT_SY_ANGLE,
        BT_ATTACK_RESET_TIME,
        BT_USE_HEAD_TRACKING,
        BT_INITIAL_STATE,
        BT_TURNING_SPEED,
        BT_FLAP_BEHAVIOR,
        BT_UPGRADE_1_SLOT_0,
        BT_UPGRADE_2_SLOT_0,
        BT_WEAPON_SLOT_0,
        BT_WEAPON_SLOT_1,
        BT_WEAPON_SLOT_2,
        BT_WEAPON_SLOT_3,
        BT_WEAPON_SLOT_4,
        BT_WEAPON_SLOT_5,
        BT_WEAPON_SLOT_6,
        BT_WEAPON_SLOT_7,
        BT_STARTS_OFF_WITH_WEAPON_0,
        BT_STARTS_OFF_WITH_WEAPON_1,
        BT_STARTS_OFF_WITH_WEAPON_2,
        BT_STARTS_OFF_WITH_WEAPON_3,
        BT_STARTS_OFF_WITH_WEAPON_4,
        BT_STARTS_OFF_WITH_WEAPON_5,
        BT_STARTS_OFF_WITH_WEAPON_6,
        BT_STARTS_OFF_WITH_WEAPON_7,
        BT_ALWAYS_RUN,
        BT_DEF_TRANS_BLEND,

        //Unknown
        BT_DUMMIES,
        BT_DEFT,
        BT_COLS,
        BT_VERSION,
        BT_HOTPS,
        BT_TRNS,
        BT_LINK,
        BT_LINKS,

        BT_COUNT,
    };

    BLOCK_TYPE GetBlockTypeFromID(const std::string& ID);

    class Block
    {
        public:
            Block() {}
            ~Block() {}

            bool Load(ByteStream* Data);
            bool Save(ByteStream* Data);

            void AddChildBlock(Block* b) { m_Children.push_back(b); }
            size_t GetChildCount() const { return m_Children.size(); }
            Block* GetChild(size_t Idx) const { return m_Children[Idx]; }
            BLOCK_TYPE GetType() const { return m_Type; }
            std::string GetTypeString() const { return m_BlockID; }

            ByteStream* GetData() const { return m_Data; }

        protected:
            unsigned char m_PreBlockFlag;
            char m_Hdr[8];
            std::string m_BlockID;
            BLOCK_TYPE m_Type;
            ByteStream* m_Data;
            std::vector<Block*> m_Children;
    };
}

#endif
