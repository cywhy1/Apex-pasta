#pragma once

// Core
constexpr uint64_t OFF_LEVEL = 0x01BA3AF4;                         //[Miscellaneous]->LevelName
constexpr uint64_t OFF_LOCAL_PLAYER = 0x026BCD68;                 //[Miscellaneous]->LocalPlayer
constexpr uint64_t OFF_ENTITY_LIST = 0x61e51c8;                 //[Miscellaneous]->cl_entitylist
constexpr uint64_t OFF_OBSERVER_LIST = 0x61e71e8;


constexpr uint64_t OFF_NAME_LIST = 0x8bf7160;                     //[Miscellaneous]->NameList
constexpr uint64_t OFF_NAME_INDEX = 0x38;                         //nameIndex

// HUD
constexpr uint64_t OFF_VIEWRENDER = 0x3cbcd98;                    //[Miscellaneous]->ViewRenderer
constexpr uint64_t OFF_VIEWMATRIX = 0x11a350;                     //[Miscellaneous]->ViewMatrix

// Player
constexpr uint64_t OFF_HEALTH = 0x0324;                           //[RecvTable.DT_Player]->m_iHealth
constexpr uint64_t OFF_MAXHEALTH = 0x0470;                        //[RecvTable.DT_Player]->m_iMaxHealth
constexpr uint64_t OFF_SHIELD = 0x01a0;                           //[RecvTable.DT_TitanSoul]->m_shieldHealth
constexpr uint64_t OFF_MAXSHIELD = 0x01a4;                        //[RecvTable.DT_TitanSoul]->m_shieldHealthMax
constexpr uint64_t OFF_OBSERVER_LIST_IN_ARRAY = 0x954;			// [RecvTable.DT_GlobalNonRewinding].m_gameTimescale

constexpr uint64_t OFF_INATTACK = 0x3cbce98;                    //[Buttons]->in_attack

constexpr uint64_t OFF_CAMERAORIGIN = 0x1fac;                     //[Miscellaneous]->CPlayer!camera_origin
constexpr uint64_t OFF_STUDIOHDR = 0xfe8;                        //[Miscellaneous]->CBaseAnimating!m_pStudioHdr
constexpr uint64_t OFF_BONES = 0x0da0 + 0x48;                     //m_nForceBone

constexpr uint64_t OFF_LOCAL_ORIGIN = 0x017c;                     //[DataMap.C_BaseEntity]->m_vecAbsOrigin
constexpr uint64_t OFF_ABSVELOCITY = 0x0170;                      //[DataMap.C_BaseEntity]->m_vecAbsVelocity

constexpr uint64_t OFF_ZOOMING = 0x1ca1;                          //[RecvTable.DT_Player]->m_bZooming
constexpr uint64_t OFF_TEAM_NUMBER = 0x0334;                      //[RecvTable.DT_BaseEntity]->m_iTeamNum
constexpr uint64_t OFF_NAME = 0x0479;                             //[RecvTable.DT_BaseEntity]->m_iName
constexpr uint64_t OFF_LIFE_STATE = 0x0690;                       //[RecvTable.DT_Player]->m_lifeState
constexpr uint64_t OFF_BLEEDOUT_STATE = 0x2828;                   //[RecvTable.DT_Player]->m_bleedoutState
constexpr uint64_t OFF_LAST_VISIBLE_TIME = 0x1A54;          //[RecvTable.DT_BaseCombatCharacter]
constexpr uint64_t OFF_LAST_AIMEDAT_TIME = 0x1A5C;    //[RecvTable.DT_BaseCombatCharacter]
constexpr uint64_t OFF_VIEW_ANGLES = 0x25fc - 0x14;               //[DataMap.C_Player]-> m_ammoPoolCapacity - 0x14
constexpr uint64_t OFF_PUNCH_ANGLES = 0x2500;                     //[DataMap.C_Player]->m_currentFrameLocalPlayer.m_vecPunchWeapon_Angle
constexpr uint64_t OFF_YAW = 0x2304 - 0x8;                       //m_currentFramePlayer.m_ammoPoolCount - 0x8

// Weapon 
constexpr uint64_t OFF_WEAPON_HANDLE = 0x19b4;                    //[RecvTable.DT_Player]->m_latestPrimaryWeapons
constexpr uint64_t OFF_WEAPON_INDEX = 0x1810;                     //[RecvTable.DT_WeaponX]->m_weaponNameIndex
constexpr uint64_t OFF_PROJECTILESCALE = 0x27F8;         //projectile_gravity_scale + [WeaponSettingsMeta]base
constexpr uint64_t OFF_PROJECTILESPEED = 0x27F0;         //projectile_launch_speed + [WeaponSettingsMeta]base
constexpr uint64_t OFF_OFFHAND_WEAPON = 0x19c4;                   //m_latestNonOffhandWeapons
constexpr uint64_t OFF_CURRENTZOOMFOV = 0x1630 + 0xb8;          //m_playerData + m_curZoomFOV
constexpr uint64_t OFF_TARGETZOOMFOV = 0x1650 + 0xbc;           //m_playerData + m_targetZoomFOV
constexpr uint64_t OFF_WEAPON_AMMO = 0x1600;                      //RecvTable.DT_WeaponX_LocalWeaponData -> m_ammoInClip
constexpr uint64_t OFF_RELOADING = 0x15fa;                        //[RecvTable.DT_WeaponX_LocalWeaponData]-> m_bInReload

// Glow
constexpr uint64_t OFF_GLOW_HIGHLIGHTS = 0x692f070;               //HighlightSettings
constexpr uint64_t OFF_GLOW_ENABLE = 0x30c;                       //Script_Highlight_GetCurrentContext
constexpr uint64_t OFF_GLOW_THROUGH_WALL = 0x26c;                 //Script_Highlight_SetVisibilityType
constexpr uint64_t OFF_GLOW_FIX = 0x278;
constexpr uint64_t OFF_GLOW_HIGHLIGHT_ID = 0x298;                 //[DT_HighlightSettings].m_highlightServerActiveStates    
constexpr uint64_t OFF_GLOW_HIGHLIGHT_TYPE_SIZE = 0x34;
constexpr uint64_t OFF_GLOW_DISTANCE = 0x264;

// Misc
constexpr long OFF_TIME_BASE = 0x2150;                        //m_currentFramePlayer.timeBase
constexpr long OFFSET_TRAVERSAL_START_TIME = 0x2c3c;          //[RecvTable.DT_LocalPlayerExclusive]->m_traversalStartTime
constexpr long OFFSET_TRAVERSAL_PROGRESS = 0x2c34;            //[RecvTable.DT_LocalPlayerExclusive]->m_traversalProgress
