//{{ Iruha : 2026-11-14 // char-luciel CH1a: Lu/Ciel's class ( June 32ae4080-32b4e76f, vtable 334f06fc, 0x98a0 bytes )
#ifdef SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE
#pragma once
#include "./X2GUAra.h"

class CX2GULuciel : public CX2GUUser
{
public:

	// June state ids ( char-luciel CH0 ch0_reg.tsv / ch0_state_skill.tsv ); June inserts a state at 24, so 7-130 are not March's USI_ values
	enum LUCIEL_STATE_ID
	{
#ifdef SERV_IRUHADEV_JUNE_ENUM_NAMES
		LSI_LU_DIE_FRONT                                          = 7,
		LSI_LU_DIE_BACK                                           = 8,
		LSI_LCG_START                                             = 9,
		LSI_LCN_START                                             = 9,
		LSI_LDB_START                                             = 9,
		LSI_LDN_START                                             = 9,
		LSI_LLC_START                                             = 9,
		LSI_LLD_START                                             = 9,
		LSI_START                                                 = 9,
		LSI_START_INTRUDE                                         = 10,
		LSI_LCG_WIN                                               = 11,
		LSI_LCG_WIN2                                              = 11,
		LSI_LCN_WIN                                               = 11,
		LSI_LCN_WIN2                                              = 11,
		LSI_LDB_WIN                                               = 11,
		LSI_LDB_WIN2                                              = 11,
		LSI_LDN_WIN                                               = 11,
		LSI_LDN_WIN2                                              = 11,
		LSI_LLC_WIN                                               = 11,
		LSI_LLC_WIN2                                              = 11,
		LSI_LLD_WIN                                               = 11,
		LSI_LLD_WIN2                                              = 11,
		LSI_WIN                                                   = 11,
		LSI_WIN2                                                  = 11,
		LSI_LCG_LOSE                                              = 12,
		LSI_LCG_LOSE2                                             = 12,
		LSI_LCN_LOSE                                              = 12,
		LSI_LCN_LOSE2                                             = 12,
		LSI_LDB_LOSE                                              = 12,
		LSI_LDB_LOSE2                                             = 12,
		LSI_LDN_LOSE                                              = 12,
		LSI_LDN_LOSE2                                             = 12,
		LSI_LLC_LOSE                                              = 12,
		LSI_LLC_LOSE2                                             = 12,
		LSI_LLD_LOSE                                              = 12,
		LSI_LLD_LOSE2                                             = 12,
		LSI_LOSE                                                  = 12,
		LSI_LOSE2                                                 = 12,
		LSI_LU_WAIT                                               = 13,
		LSI_LU_WALK                                               = 14,
		LSI_LU_JUMP_READY                                         = 15,
		LSI_LU_JUMP_UP                                            = 16,
		LSI_LU_JUMP_DOWN                                          = 17,
		LSI_LU_JUMP_LANDING                                       = 18,
		LSI_LU_DASH                                               = 19,
		LSI_LU_DASH_END                                           = 20,
		LSI_LU_DASH_JUMP                                          = 21,
		LSI_LU_DASH_JUMP_LANDING                                  = 22,
		LSI_LCN_LU_HYPER_MODE                                     = 23,
		LSI_LDN_LU_HYPER_MODE                                     = 23,
		LSI_LLD_LU_HYPER_MODE                                     = 23,
		LSI_LU_HYPER_MODE                                         = 23,
		LSI_LU_DAMAGE_GROGGY                                      = 25,
		LSI_LU_DAMAGE_SMALL_FRONT                                 = 26,
		LSI_LU_DAMAGE_SMALL_BACK                                  = 27,
		LSI_LU_DAMAGE_BIG_FRONT                                   = 28,
		LSI_LU_DAMAGE_BIG_BACK                                    = 29,
		LSI_LU_DAMAGE_DOWN_FRONT                                  = 30,
		LSI_LU_DAMAGE_DOWN_BACK                                   = 31,
		LSI_LU_DAMAGE_STANDUP_FRONT                               = 32,
		LSI_LU_DAMAGE_STANDUP_BACK                                = 33,
		LSI_LU_DAMAGE_AIR_SMALL                                   = 34,
		LSI_LU_DAMAGE_AIR_DOWN                                    = 35,
		LSI_LU_DAMAGE_AIR_DOWN_INVINCIBLE                         = 36,
		LSI_LU_DAMAGE_AIR_DOWN_LANDING                            = 37,
		LSI_LU_DAMAGE_AIR_FALL                                    = 38,
		LSI_LU_DAMAGE_AIR_UP                                      = 39,
		LSI_LU_DAMAGE_AIR_FLY_FRONT                               = 40,
		LSI_LU_DAMAGE_AIR_FLY_BACK                                = 41,
		LSI_LU_DAMAGE_REVENGE                                     = 42,
		LSI_RIDING_HYPER_MODE                                     = 44,
		LSI_LU_PEPPER_RUN_READY                                   = 45,
		LSI_LU_PEPPER_RUN                                         = 46,
		LSI_LU_PEPPER_RUN_END                                     = 47,
		LSI_LU_PEPPER_RUN_JUMP_UP                                 = 48,
		LSI_LU_PEPPER_RUN_JUMP_DOWN                               = 49,
		LSI_LU_THROW_ITEM                                         = 70,
		LSI_RIDING_START                                          = 72,
		LSI_RIDING_ON                                             = 73,
		LSI_RIDING_OFF                                            = 74,
		LSI_RIDING_WAIT_HABIT                                     = 75,
		LSI_RIDING_WAIT                                           = 76,
		LSI_RIDING_WALK                                           = 77,
		LSI_RIDING_JUMP_UP                                        = 78,
		LSI_RIDING_JUMP_DOWN                                      = 79,
		LSI_RIDING_JUMP_LANDING                                   = 80,
		LSI_RIDING_DASH                                           = 81,
		LSI_RIDING_DASH_END                                       = 82,
		LSI_RIDING_DASH_JUMP                                      = 83,
		LSI_RIDING_DASH_JUMP_LANDING                              = 84,
		LSI_RIDING_DAMAGE_FRONT                                   = 85,
		LSI_RIDING_DAMAGE_BACK                                    = 86,
		LSI_RIDING_DIE                                            = 87,
		LSI_RIDING_ATTACK_Z                                       = 88,
		LSI_RIDING_JUMP_ATTACK_Z                                  = 89,
		LSI_RIDING_ATTACK_X                                       = 90,
		LSI_RIDING_ATTACK_SPECIAL                                 = 91,
		LSI_RIDING_SPECIAL_MOVE                                   = 92,
		LSI_RIDING_SPECIFIC                                       = 93,
		LSI_RIDING_ATTACK_X_READY                                 = 94,
		LSI_RIDING_ATTACK_X_READY_2                               = 95,
		LSI_RIDING_ATTACK_X_END                                   = 96,
		LSI_LU_DAMAGE_BOUNDING_DOWN                               = 120,
		LSI_LU_DAMAGE_BOUNDING_LANDCRASH                          = 121,
		LSI_LU_DAMAGE_BOUNDING_UP                                 = 122,
		LSI_LU_AMUSEMENT_ITEM_PHOENIX_RUN                         = 123,
		LSI_LU_AMUSEMENT_ITEM_PHOENIX_RUN_END                     = 126,
		LSI_LU_DANCE_Z                                            = 127,
		LSI_LU_DANCE_X                                            = 128,
		LSI_LU_DANCE_WAIT                                         = 129,
		LSI_WAIT_STATE_CHANGE                                     = 130,
		LSI_LU_DASH_JUMP_POWER_LANDING                            = 154,
		LSI_LU_STANDUP_ROLLING_FRONT_FRONT                        = 155,
		LSI_LU_STANDUP_ROLLING_FRONT_BACK                         = 156,
		LSI_LU_STANDUP_ROLLING_BACK_FRONT                         = 157,
		LSI_LU_STANDUP_ROLLING_BACK_BACK                          = 158,
		LSI_LU_STANDUP_ATTACK_FRONT                               = 159,
		LSI_LU_STANDUP_ATTACK_BACK                                = 160,
		LSI_LU_STANDUP_ATTACK_FRONT_NO_DOWN                       = 161,
		LSI_LU_STANDUP_ATTACK_BACK_NO_DOWN                        = 162,
		LSI_LU_SWITCHING                                          = 163,
		LSI_LU_COMBO_Z                                            = 164,
		LSI_LU_COMBO_ZZ                                           = 165,
		LSI_LU_COMBO_ZZZ                                          = 166,
		LSI_LU_COMBO_ZZZZ                                         = 167,
		LSI_LU_COMBO_ZZZZX                                        = 168,
		LSI_LU_COMBO_X                                            = 169,
		LSI_LU_COMBO_XX                                           = 170,
		LSI_LU_COMBO_XXX                                          = 171,
		LSI_LU_COMBO_XXXX                                         = 172,
		LSI_LU_COMBO_XXXZ_READY                                   = 173,
		LSI_LU_COMBO_XXXZ_CHARGE                                  = 174,
		LSI_LU_COMBO_XXXZ_LOOP                                    = 175,
		LSI_LU_COMBO_XXXZ_END                                     = 176,
		LSI_LU_COMBO_XXXZX                                        = 177,
		LSI_LU_JUMP_COMBO_Z                                       = 178,
		LSI_LU_JUMP_COMBO_X                                       = 179,
		LSI_LU_DASH_COMBO_Z                                       = 180,
		LSI_LU_DASH_COMBO_X_START                                 = 181,
		LSI_LU_DASH_COMBO_X_LANDING                               = 182,
		LSI_LU_DASH_JUMP_COMBO_Z                                  = 183,
		LSI_LU_DASH_JUMP_COMBO_ZX                                 = 184,
		LSI_LU_DASH_JUMP_COMBO_X                                  = 185,
		LSI_LU_DASH_JUMP_COMBO_XX_START                           = 186,
		LSI_LU_DASH_JUMP_COMBO_XX_END                             = 187,
		LSI_LU_DASH_JUMP_COMBO_XXX                                = 188,
		LSI_LU_DASH_JUMP_COMBO_XXXX                               = 189,
		LSI_LU_DASH_JUMP_COMBO_XXXXZ                              = 190,
		LSI_SA_LD_STOMP_LANDING                                   = 191,
		LSI_CIEL_DIE_FRONT                                        = 192,
		LSI_CIEL_DIE_BACK                                         = 193,
		LSI_CIEL_WAIT                                             = 194,
		LSI_CIEL_WALK                                             = 195,
		LSI_CIEL_JUMP_READY                                       = 196,
		LSI_CIEL_JUMP_UP                                          = 197,
		LSI_CIEL_JUMP_DOWN                                        = 198,
		LSI_CIEL_JUMP_LANDING                                     = 199,
		LSI_CIEL_DASH                                             = 200,
		LSI_CIEL_DASH_END                                         = 201,
		LSI_CIEL_DASH_JUMP                                        = 202,
		LSI_CIEL_DASH_JUMP_LANDING                                = 203,
		LSI_CIEL_HYPER_MODE                                       = 204,
		LSI_LCN_CIEL_HYPER_MODE                                   = 204,
		LSI_LDN_CIEL_HYPER_MODE                                   = 204,
		LSI_LLD_CIEL_HYPER_MODE                                   = 204,
		LSI_CIEL_DAMAGE_GROGGY                                    = 205,
		LSI_CIEL_DAMAGE_SMALL_FRONT                               = 206,
		LSI_CIEL_DAMAGE_SMALL_BACK                                = 207,
		LSI_CIEL_DAMAGE_BIG_FRONT                                 = 208,
		LSI_CIEL_DAMAGE_BIG_BACK                                  = 209,
		LSI_CIEL_DAMAGE_DOWN_FRONT                                = 210,
		LSI_CIEL_DAMAGE_DOWN_BACK                                 = 211,
		LSI_CIEL_DAMAGE_STANDUP_FRONT                             = 212,
		LSI_CIEL_DAMAGE_STANDUP_BACK                              = 213,
		LSI_CIEL_DAMAGE_AIR_SMALL                                 = 214,
		LSI_CIEL_DAMAGE_AIR_DOWN                                  = 215,
		LSI_CIEL_DAMAGE_AIR_DOWN_INVINCIBLE                       = 216,
		LSI_CIEL_DAMAGE_AIR_DOWN_LANDING                          = 217,
		LSI_CIEL_DAMAGE_AIR_FALL                                  = 218,
		LSI_CIEL_DAMAGE_AIR_UP                                    = 219,
		LSI_CIEL_DAMAGE_AIR_FLY_FRONT                             = 220,
		LSI_CIEL_DAMAGE_AIR_FLY_BACK                              = 221,
		LSI_CIEL_DAMAGE_REVENGE                                   = 222,
		LSI_CIEL_PEPPER_RUN_READY                                 = 223,
		LSI_CIEL_PEPPER_RUN                                       = 224,
		LSI_CIEL_PEPPER_RUN_END                                   = 225,
		LSI_CIEL_PEPPER_RUN_JUMP_UP                               = 226,
		LSI_CIEL_PEPPER_RUN_JUMP_DOWN                             = 227,
		LSI_CIEL_THROW_ITEM                                       = 228,
		LSI_CIEL_DASH_JUMP_POWER_LANDING                          = 230,
		LSI_CIEL_STANDUP_ROLLING_FRONT_FRONT                      = 231,
		LSI_CIEL_STANDUP_ROLLING_FRONT_BACK                       = 232,
		LSI_CIEL_STANDUP_ROLLING_BACK_FRONT                       = 233,
		LSI_CIEL_STANDUP_ROLLING_BACK_BACK                        = 234,
		LSI_CIEL_STANDUP_ATTACK_FRONT                             = 235,
		LSI_CIEL_STANDUP_ATTACK_BACK                              = 236,
		LSI_CIEL_SWITCHING                                        = 237,
		LSI_CIEL_COMBO_Z                                          = 238,
		LSI_CIEL_COMBO_ZZ                                         = 239,
		LSI_CIEL_COMBO_ZZZ                                        = 240,
		LSI_CIEL_COMBO_ZZZZ                                       = 241,
		LSI_CIEL_COMBO_ZZZZX                                      = 242,
		LSI_CIEL_COMBO_X                                          = 243,
		LSI_CIEL_COMBO_XX                                         = 244,
		LSI_CIEL_COMBO_XXX                                        = 245,
		LSI_CIEL_COMBO_XXXX                                       = 246,
		LSI_CIEL_COMBO_XXXZ_START                                 = 247,
		LSI_CIEL_COMBO_XXXZ_LOOP                                  = 248,
		LSI_CIEL_COMBO_XXXZ_END                                   = 249,
		LSI_CIEL_COMBO_XXXZX                                      = 250,
		LSI_CIEL_JUMP_COMBO_Z                                     = 251,
		LSI_CIEL_JUMP_COMBO_X                                     = 252,
		LSI_CIEL_DASH_COMBO_Z                                     = 253,
		LSI_CIEL_DASH_COMBO_X                                     = 254,
		LSI_CIEL_DASH_JUMP_COMBO_Z                                = 255,
		LSI_CIEL_DASH_JUMP_COMBO_ZX                               = 256,
		LSI_CIEL_DASH_JUMP_COMBO_X                                = 257,
		LSI_CIEL_DASH_JUMP_COMBO_XX                               = 258,
		LSI_CIEL_DASH_JUMP_COMBO_XXX                              = 259,
		LSI_CIEL_DASH_JUMP_COMBO_XXXX                             = 260,
		LSI_CIEL_DASH_JUMP_COMBO_XXXXZ                            = 261,
		LSI_CIEL_DASH_JUMP_COMBO_XXXXZ_LANDING                    = 262,
		LSI_SA_M_COMMON_AURA_DEPRAVITY_ACCEL                      = 264,
		LSI_SA_M_COMMON_AURA_PUNISHING_ACCEL                      = 265,
		LSI_A_LD_M_HEAVY_HIT                                      = 266,
		LSI_A_LD_M_HAUNTING_BEAT                                  = 267,
		LSI_LLC_COMBO_ZZZX                                        = 269,
		LSI_LLC_DASH_COMBO_Z                                      = 270,
		LSI_LLC_DASH_COMBO_ZX                                     = 271,
		LSI_LLC_DASH_COMBO_ZXX                                    = 272,
		LSI_LLC_DASH_COMBO_XX                                     = 273,
		LSI_LLC_F_DASH_JUMP_COMBO_Z_DASH                          = 274,
		LSI_LLC_F_DASH_JUMP_COMBO_Z_DASH_UP                       = 275,
		LSI_LLC_F_DASH_JUMP_COMBO_Z_DASH_DOWN                     = 276,
		LSI_LLC_F_DASH_JUMP_COMBO_X_DASH                          = 277,
		LSI_LLC_F_DASH_JUMP_COMBO_X_DASH_UP                       = 278,
		LSI_LLC_F_DASH_JUMP_COMBO_X_DASH_DOWN                     = 279,
		LSI_LU_JUMP_DOWN_NO_ATTACK                                = 280,
		LSI_LLC_F_DASH_COMBO_ZXXX                                 = 281,
		LSI_LLC_F_DASH_COMBO_XXX                                  = 282,
		LSI_LLC_M_DASH_JUMP_COMBO_Z_DASH                          = 283,
		LSI_LLC_M_DASH_JUMP_COMBO_Z_DASH_UP                       = 284,
		LSI_LLC_M_DASH_JUMP_COMBO_Z_DASH_DOWN                     = 285,
		LSI_LLC_M_DASH_JUMP_COMBO_X_DASH                          = 286,
		LSI_LLC_M_DASH_JUMP_COMBO_X_DASH_UP                       = 287,
		LSI_LLC_M_DASH_JUMP_COMBO_X_DASH_DOWN                     = 288,
		LSI_CIEL_JUMP_DOWN_NO_ATTACK                              = 289,
		LSI_LLC_M_COMBO_ZZZX                                      = 290,
		LSI_LLC_M_DASH_COMBO_ZX                                   = 291,
		LSI_LLC_M_DASH_COMBO_ZXX                                  = 292,
		LSI_LLC_M_DASH_COMBO_ZXXX                                 = 293,
		LSI_LLC_M_DASH_COMBO_XX                                   = 294,
		LSI_LLC_M_DASH_COMBO_XXX                                  = 295,
		LSI_A_LLC_M_BLASTING_FEAR                                 = 296,
		LSI_A_LLC_M_BACKSTAB                                      = 297,
		LSI_SA_LLC_DARK_CRESCENT                                  = 298,
		LSI_SA_LLC_RUINOUS_IMPACT                                 = 299,
		LSI_LCG_COMBO_ZZZX                                        = 300,
		LSI_LCG_COMBO_XXXX                                        = 301,
		LSI_LCG_DASH_COMBO_ZX                                     = 302,
		LSI_LCG_DASH_COMBO_ZXX                                    = 303,
		LSI_LCG_COMBO_SNIPING_READY                               = 304,
		LSI_LCG_COMBO_SNIPING_Z                                   = 305,
		LSI_LCG_COMBO_SNIPING_Z_FINISH                            = 306,
		LSI_LCG_COMBO_SNIPING_X_CHARGE                            = 307,
		LSI_LCG_COMBO_SNIPING_X_FIRE                              = 308,
		LSI_LCG_DASH_JUMP_COMBO_SNIPING_START                     = 309,
		LSI_LCG_DASH_JUMP_COMBO_SNIPING_LOOP                      = 310,
		LSI_LCG_DASH_JUMP_COMBO_SNIPING_FINISH                    = 311,
		LSI_LCG_F_COMBO_ZZZX                                      = 312,
		LSI_LCG_F_COMBO_ZZZX_WAIT                                 = 313,
		LSI_LCG_F_COMBO_ZZZXX_LOOP                                = 314,
		LSI_LCG_F_COMBO_ZZZXX_END                                 = 315,
		LSI_LCG_F_COMBO_ZZZXZ_LOOP                                = 316,
		LSI_LCG_F_COMBO_ZZZXZ_END                                 = 317,
		LSI_LCG_F_COMBO_XXXX                                      = 318,
		LSI_LCG_F_COMBO_XXXXX_LOOP                                = 319,
		LSI_LCG_F_COMBO_XXXXX_END                                 = 320,
		LSI_LCG_F_COMBO_XXXXZ_LOOP                                = 321,
		LSI_LCG_F_COMBO_XXXXZ_END                                 = 322,
		LSI_LCG_F_DASH_COMBO_Z                                    = 323,
		LSI_LCG_F_DASH_COMBO_ZX                                   = 324,
		LSI_LCG_F_DASH_COMBO_ZXX                                  = 325,
		LSI_LCG_F_DASH_COMBO_ZXXZ_LOOP                            = 326,
		LSI_LCG_F_DASH_COMBO_ZXXZ_END                             = 327,
		LSI_LCG_F_DASH_COMBO_ZXXX_LOOP                            = 328,
		LSI_LCG_F_DASH_COMBO_ZXXX_END                             = 329,
		LSI_LCG_F_DASH_COMBO_XX                                   = 330,
		LSI_LCG_F_DASH_COMBO_XXX                                  = 331,
		LSI_LCG_F_DASH_COMBO_XXXZ_LOOP                            = 332,
		LSI_LCG_F_DASH_COMBO_XXXZ_END                             = 333,
		LSI_LCG_F_DASH_COMBO_XXXX_LOOP                            = 334,
		LSI_LCG_F_DASH_COMBO_XXXX_END                             = 335,
		LSI_LCG_F_DASH_JUMP_COMBO_ZXX                             = 336,
		LSI_LCG_M_DASH_JUMP_COMBO_ZXX                             = 337,
		LSI_LCG_F_DASH_JUMP_COMBO_XZZ_READY                       = 339,
		LSI_LCG_F_DASH_JUMP_COMBO_XZZ_LOOP                        = 340,
		LSI_LCG_F_DASH_JUMP_COMBO_XZZ_END                         = 341,
		LSI_A_LCG_F_SOUL_STRIKE_LOOP                              = 342,
		LSI_A_LCG_F_SOUL_STRIKE_END                               = 343,
		LSI_A_LCG_M_SOUL_SHOCK                                    = 344,
		LSI_A_LCG_M_SOUL_STRIKE_READY                             = 345,
		LSI_A_LCG_M_SOUL_STRIKE_LOOP                              = 346,
		LSI_A_LCG_M_SOUL_STRIKE_END                               = 347,
		LSI_SA_LCG_DEMONIC_BREATH_CHARGING                        = 348,
		LSI_SA_LCG_DEMONIC_BREATH_FIRE                            = 349,
		LSI_SA_LCG_FANTASIE_IMPROMPTU_CANTABILE_WAIT              = 350,
		LSI_SA_LCG_FANTASIE_IMPROMPTU_CANTABILE_FRONT_MOVE        = 351,
		LSI_SA_LCG_FANTASIE_IMPROMPTU_CANTABILE_BACK_MOVE         = 352,
		LSI_SA_LCG_FANTASIE_IMPROMPTU_ACCELERANDO_READY           = 353,
		LSI_SA_LCG_FANTASIE_IMPROMPTU_ACCELERANDO_WAIT            = 354,
		LSI_SA_LCG_FANTASIE_IMPROMPTU_ACCELERANDO_FRONT_MOVE      = 355,
		LSI_SA_LCG_FANTASIE_IMPROMPTU_ACCELERANDO_BACK_MOVE       = 356,
		LSI_SA_LCG_FANTASIE_IMPROMPTU_ACCELERANDO_END             = 357,
		LSI_SA_LCG_DEATH_CRISIS_OF_MARBAS_WAIT                    = 358,
		LSI_SA_LCG_DEATH_CRISIS_OF_MARBAS_SMALL_FIRE              = 359,
		LSI_SA_LCG_DEATH_CRISIS_OF_MARBAS_BIG_CHARGE              = 360,
		LSI_SA_LCG_DEATH_CRISIS_OF_MARBAS_BIG_FIRE                = 361,
		LSI_SA_LCG_DEATH_CRISIS_OF_MARBAS_END                     = 362,
		LSI_SA_LCG_ESCHATON                                       = 363,
		LSI_SA_LCG_FANTASIE_IMPROMPTU_2ND_MOVEMENT                = 364,
		LSI_LLD_COMBO_ZZZXZ                                       = 365,
		LSI_LLD_COMBO_XX_DOWN_X                                   = 366,
		LSI_LLD_COMBO_XX_DOWN_XX_READY                            = 367,
		LSI_LLD_COMBO_XX_DOWN_XX_LOOP                             = 368,
		LSI_LLD_COMBO_XX_DOWN_XX_LOOP_X                           = 369,
		LSI_LLD_M_COMBO_ZZZXZ                                     = 370,
		LSI_LLD_M_COMBO_XX_DOWN_X                                 = 371,
		LSI_LLD_M_COMBO_XX_DOWN_XX_READY                          = 372,
		LSI_LLD_M_COMBO_XX_DOWN_XX_LOOP                           = 373,
		LSI_LLD_M_COMBO_XX_DOWN_XX_END                            = 374,
		LSI_A_LLD_F_CHAINS_OF_BONDAGE_DASH_ATTACK                 = 376,
		LSI_A_LLD_M_FEAR_ABSORPTION                               = 377,
		LSI_A_LLD_M_CHAINS_OF_BONDAGE                             = 378,
		LSI_A_LLD_M_CHAINS_OF_BONDAGE_DASH_ATTACK                 = 379,
		LSI_SA_LLD_DANCING_BLADE_LOOP                             = 380,
		LSI_SA_LLD_PHANTOM_BLADE_ATTACK                           = 381,
		LSI_SA_LLD_PHANTOM_BLADE_FAIL                             = 382,
		LSI_LCN_DASH_COMBO_ZX_DOWN_X                              = 383,
		LSI_LCN_DASH_COMBO_ZX_DOWN_XX_LOOP                        = 384,
		LSI_LCN_DASH_COMBO_ZX_DOWN_XX_END                         = 385,
		LSI_LCN_SNIPING_COMBO_UP_X                                = 386,
		LSI_LCN_SNIPING_COMBO_DOWN_X                              = 387,
		LSI_LCN_F_COMBO_ZZZX_UP_X                                 = 388,
		LSI_LCN_F_COMBO_ZZZX_DOWN_X                               = 389,
		LSI_LCN_F_COMBO_XXXX_UP_X                                 = 390,
		LSI_LCN_F_COMBO_XXXX_DOWN_X                               = 391,
		LSI_LCN_F_DASH_COMBO_ZXX_UP_X                             = 392,
		LSI_LCN_F_DASH_COMBO_ZXX_DOWN_X                           = 393,
		LSI_LCN_F_DASH_COMBO_XXX_UP_X                             = 394,
		LSI_LCN_F_DASH_COMBO_XXX_DOWN_X                           = 395,
		LSI_LCN_F_DASH_COMBO_ZX_DOWN_X                            = 396,
		LSI_LCN_F_DASH_COMBO_ZX_DOWN_XX_LOOP                      = 397,
		LSI_LCN_F_DASH_COMBO_ZX_DOWN_XX_END                       = 398,
		LSI_LCN_F_DASH_COMBO_XX_DOWN_X                            = 399,
		LSI_LCN_F_DASH_COMBO_XX_DOWN_XX_LOOP                      = 400,
		LSI_LCN_F_DASH_COMBO_XX_DOWN_XX_END                       = 401,
		LSI_A_LCN_M_SUMMON_FAMILIAR                               = 402,
		LSI_A_LCN_M_SHADOW                                        = 403,
		LSI_SA_LCN_M_SWIFT_SMASH                                  = 404,
		LSI_SA_LCN_SHADOW_BOLT_CONTROL                            = 405,
		LSI_SA_LCN_SHADOW_BOLT_END                                = 414,
		LSI_SA_LCN_F_TWILIGHT_JUDGEMENT_REATTACK                  = 415,
		LSI_SA_LCN_M_TWILIGHT_JUDGEMENT_REATTACK                  = 416,
		LSI_CIEL_DAMAGE_BOUNDING_DOWN                             = 417,
		LSI_CIEL_DAMAGE_BOUNDING_LANDCRASH                        = 418,
		LSI_CIEL_DAMAGE_BOUNDING_UP                               = 419,
		LSI_SA_LDB_LAND_CRASHER_LANDING                           = 420,
		LSI_SA_LDB_LAND_CRASHER_CHAIN                             = 421,
		LSI_SA_LDB_NIGHTMARE_TARGET                               = 422,
		LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_START                  = 423,
		LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_WAIT                   = 424,
		LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_WALK                   = 425,
		LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_JUMP_READY             = 426,
		LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_JUMP_UP                = 427,
		LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_JUMP_DOWN              = 428,
		LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_JUMP_LANDING           = 429,
		LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_DASH                   = 430,
		LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_DASH_END               = 431,
		LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_DASH_JUMP              = 432,
		LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_DASH_JUMP_LANDING      = 433,
		LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_JUMP_Z                 = 434,
		LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_JUMP_X                 = 435,
		LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_COMBO_Z                = 436,
		LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_COMBO_ZZ               = 437,
		LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_COMBO_ZZZ              = 438,
		LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_COMBO_ZZZZ             = 439,
		LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_COMBO_X                = 440,
		LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_COMBO_XX               = 441,
		LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_COMBO_XXX_READY        = 442,
		LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_COMBO_XXX_END          = 443,
		LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_DEATH_TOUCH            = 446,
		LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_MORTAL_FEAR            = 447,
		LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_DARK_PLASMA            = 448,
		LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_HYPER_MODE             = 449,
		LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_HYPER_MODE_TR          = 450,
		LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_THROW_ITEM             = 451,
		LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_END                    = 452,
		LSI_SA_LDB_ALTAR_OF_EVIL_END                              = 453,
		LSI_SA_LDN_DESPERADO_ATTACK_READY                         = 454,
		LSI_SA_LDN_DESPERADO_ATTACK                               = 455,
		LSI_SA_LDN_DESPERADO_ATTACK_END                           = 456,
		LSI_SA_LDN_DESPERADO_ATTACK_UP                            = 457,
		LSI_SA_LDN_DESPERADO_ATTACK_DOWN                          = 458,
		LSI_A_LDB_M_ABYSS_STRIKE                                  = 459,
		LSI_A_LDB_F_PHANTOM_RUSH_TARGET                           = 460,
		LSI_A_LDB_M_PHANTOM_RUSH                                  = 461,
		LSI_A_LDB_M_PHANTOM_RUSH_TARGET                           = 462,
		LSI_SA_M_COMMON_BUFF_PHANTOM_CIRCLE                       = 463,
		LSI_A_LDN_F_RAPID_IMPACT_MOVEATTACK                       = 464,
		LSI_A_LDN_M_RAPID_IMPACT                                  = 465,
		LSI_A_LDN_M_RAPID_IMPACT_MOVEATTACK                       = 466,
		LSI_A_LDN_M_PUNISHER                                      = 467,
		LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_START                  = 468,
		LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_WAIT                   = 469,
		LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_WALK                   = 470,
		LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_JUMP_READY             = 471,
		LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_JUMP_UP                = 472,
		LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_JUMP_DOWN              = 473,
		LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_JUMP_LANDING           = 474,
		LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_DASH                   = 475,
		LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_DASH_END               = 476,
		LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_DASH_JUMP              = 477,
		LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_DASH_JUMP_LANDING      = 478,
		LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_JUMP_Z                 = 479,
		LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_JUMP_Z_LOOP            = 480,
		LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_JUMP_Z_LANDING         = 481,
		LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_JUMP_X                 = 482,
		LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_JUMP_X_LOOP            = 483,
		LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_JUMP_X_LANDING         = 484,
		LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_COMBO_Z                = 485,
		LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_COMBO_ZZ               = 486,
		LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_COMBO_ZZZ              = 487,
		LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_COMBO_ZZZZ             = 488,
		LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_COMBO_X                = 489,
		LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_COMBO_XX               = 490,
		LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_COMBO_XXX_READY        = 491,
		LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_COMBO_XXX_LOOP         = 492,
		LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_COMBO_XXX_END          = 493,
		LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_SHADOW_FURY            = 494,
		LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_SHREWD_CUTTER          = 495,
		LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_SHREWD_CUTTER_ATTACK_A = 496,
		LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_SHREWD_CUTTER_ATTACK_B = 497,
		LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_SHREWD_CUTTER_END      = 498,
		LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_SILENT_RAGE            = 500,
		LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_HYPER_MODE             = 501,
		LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_HYPER_MODE_TR          = 502,
		LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_THROW_ITEM             = 503,
		LSI_SA_LDN_TRANSCENDENCE_TRANSFORM_END                    = 504,
		LSI_SA_LDN_TRANSCENDENCE_END                              = 505,
		LSI_BOUNDING_TARGET_LU_STRIKER                            = 509,
		LSI_BOUNDING_TARGET_CIEL_STRIKER                          = 510,
		LSI_BOUNDING_TARGET_LU_COMMAND_X                          = 511,
		LSI_BOUNDING_TARGET_LU_COMMAND_X_CIEL_STRIKER             = 512,
		LSI_BOUNDING_TARGET_LU_COMMAND_XX                         = 513,
		LSI_BOUNDING_TARGET_LU_COMMAND_XXX                        = 514,
		LSI_BOUNDING_TARGET_LU_COMMAND_XXXX                       = 515,
		LSI_BOUNDING_TARGET_LU_COMMAND_XXXXX                      = 516,
		LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXX                     = 517,
		LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX                    = 518,
		LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_Z_START            = 519,
		LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_Z_LOOP             = 520,
		LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_Z_LANDING          = 521,
		LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_ZZ                 = 522,
		LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_ZZZ                = 523,
		LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_ZZZX               = 524,
		LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_X                  = 525,
		LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_XX_START           = 526,
		LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_XX_LOOP            = 527,
		LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_XX_LANDING         = 528,
		LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_XXZ                = 529,
		LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_XXZX               = 530,
		LSI_BOUNDING_TARGET_CIEL_COMMAND_X                        = 531,
		LSI_BOUNDING_TARGET_CIEL_COMMAND_X_LU_STRIKER             = 532,
		LSI_BOUNDING_TARGET_CIEL_COMMAND_XX                       = 533,
		LSI_BOUNDING_TARGET_CIEL_COMMAND_XXX                      = 534,
		LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXX                     = 535,
		LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXX                    = 536,
		LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXX                   = 537,
		LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX                  = 538,
		LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_Z_START          = 539,
		LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_Z_LOOP           = 540,
		LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_Z_END            = 541,
		LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_ZZ               = 542,
		LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_ZZZ              = 543,
		LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_ZZZX             = 544,
		LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_X                = 545,
		LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_XX_START         = 546,
		LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_XX_LOOP          = 547,
		LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_XX_LANDING       = 548,
		LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_XXZ              = 549,
		LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_XXZX             = 550,
		LSI_LDB_F_DASH_COMBO_Z                                    = 551,
		LSI_LDB_F_DASH_COMBO_ZX                                   = 552,
		LSI_LDB_F_DASH_COMBO_ZXX                                  = 553,
		LSI_LDB_F_DASH_COMBO_ZXXX                                 = 554,
		LSI_LDB_F_DASH_COMBO_ZXXXX                                = 555,
		LSI_LDB_F_DASH_COMBO_ZXXXXZ                               = 556,
		LSI_LDB_M_DASH_COMBO_ZX                                   = 557,
		LSI_LDB_M_DASH_COMBO_ZXX                                  = 558,
		LSI_LDB_M_DASH_COMBO_ZXXX                                 = 559,
		LSI_LDB_M_DASH_COMBO_ZXXXX                                = 560,
		LSI_LDB_M_DASH_COMBO_ZXXXXZ                               = 561,
		LSI_LDB_F_DASH_JUMP_COMBO_XZ_START                        = 562,
		LSI_LDB_F_DASH_JUMP_COMBO_XZ_LANDING                      = 563,
		LSI_LDB_F_DASH_JUMP_COMBO_XZ_LANDING_ATTACK               = 564,
		LSI_LDB_F_DASH_JUMP_COMBO_XZX                             = 565,
		LSI_LDB_M_DASH_JUMP_COMBO_XZ_START                        = 567,
		LSI_LDB_M_DASH_JUMP_COMBO_XZ_LANDING                      = 568,
		LSI_LDB_M_DASH_JUMP_COMBO_XZ_LANDING_ATTACK               = 569,
		LSI_LDB_M_DASH_JUMP_COMBO_XZX                             = 570,
		LSI_CIEL_AMUSEMENT_ITEM_PHOENIX_RUN                       = 571,
		LSI_CIEL_AMUSEMENT_ITEM_PHOENIX_RUN_END                   = 574,
		LSI_A_LLD_BLUFF_CIEL                                      = 575,
		LSI_A_LCN_SOUL_SPREAD_LU_LOOP                             = 576,
		LSI_A_LCN_SOUL_SPREAD_LU_END                              = 577,
		LSI_A_LCN_SOUL_SPREAD_CIEL_READY                          = 578,
		LSI_A_LCN_SOUL_SPREAD_CIEL_LOOP                           = 579,
		LSI_A_LCN_SOUL_SPREAD_CIEL_END                            = 580,
		LSI_A_LDN_RAPID_FURY_CIEL                                 = 581,
		LSI_SA_LDN_DEMONIC_RUSH_LAST                              = 582,
		LSI_SA_LDN_DEMONIC_RUSH_LANDING                           = 583,
		LSI_SA_LCN_MAGDONIC_CANNON_FLY                            = 584,
		LSI_SA_LDN_ABYSS_SNIPING_UP                               = 585,
		LSI_SA_LDN_ABYSS_SNIPING_ATTACK                           = 586,
		LSI_SA_LDN_ABYSS_SNIPING_DOWN                             = 587,
		LSI_SA_LDN_ABYSS_SNIPING_END                              = 588,
		LSI_SA_LDN_SINISTER_MIND_FLY                              = 589,
		LSI_CIEL_DANCE_Z                                          = 590,
		LSI_CIEL_DANCE_X                                          = 591,
		LSI_CIEL_DANCE_WAIT                                       = 592,
#endif SERV_IRUHADEV_JUNE_ENUM_NAMES
	};

	enum LUCIEL_MAJOR_PARTICLE_INSTANCE_ID
	{
		LUCIEL_MAJOR_PII_BASE,
		LUCIEL_MAJOR_PII_END,
	};

	enum LUCIEL_MINOR_PARTICLE_INSTANCE_ID
	{
		LUCIEL_MINOR_PII_BASE,
		LUCIEL_MINOR_PII_END = 7,	// June +0x95a0: seven handles, named by the phases that use them
	};

	// June's { UID, GAME_UNIT_TYPE } unit key ( 16 bytes; looked up by 32294460 )
	struct SGameUnitKey
	{
		UidType							m_uidGameUnit;
		CX2GameUnit::GAME_UNIT_TYPE		m_eGameUnitType;

		SGameUnitKey() : m_uidGameUnit( -1 ), m_eGameUnitType( CX2GameUnit::GUT_USER ) {}
	};

	// June 24-byte entry: a time and a unit key ( +0x97d4, +0x97ec )
	struct STimedGameUnitKey
	{
		float							m_fTime;
		SGameUnitKey					m_GameUnitKey;
	};

	// June 12-byte entry of +0x9890: the hits a damage effect of a skill made ( CountDamageEffectHit )
	struct SDamageEffectHitCount
	{
		int								m_iSkillID;
		int								m_iDamageEffectIndex;
		int								m_iHitCount;
	};

	// June +0x9748, member initializer 32b4d1c0, destructor 32699670 ( Altar of Evil, CH2d2 )
	struct SAltarOfEvilData
	{
		float								m_fDuration;
		float								m_fRange;
		float								m_fInterval;
		float								m_fElapsedTime;
		std::vector<CX2EffectSet::Handle>	m_vecEffectSet;
		std::vector<D3DXVECTOR3>			m_vecPosition;

		SAltarOfEvilData() : m_fDuration( 2.f ), m_fRange( 1200.f ), m_fInterval( 0.2f ), m_fElapsedTime( 0.f ) {}
	};

	virtual void		CountDamageEffectHit( const CX2DamageManager::DamageData* pDamageData_ );	// own virtual 0x880, June 32b14db0
	virtual void		TryUseSkillWithCombinationPoint( const CX2SkillTree::SKILL_ID eSkillID_,
							const CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseCondition_ );	// own virtual 0x884, June 32b3acf0
	virtual void		PreloadAuraAccelResources();	// own virtual 0x888, June 32ae6130

private:
	CX2GULuciel( int iUnitIndex_, int iTeamNum_,
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		lua_State*   pScriptLuaState,
#else	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		const WCHAR* pScriptFileName,
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		int iFrameBufferNum_, IN CX2Unit* pUnit_ );
	virtual ~CX2GULuciel();

protected:
	void				InitializeLucielMajorParticleArray();
	void				InitializeLucielMinorParticleArray();
	void				DeleteLucielMinorParticle();	// June 32b1eb10
	void				SetSkillCutInSetIndexByUnitClass();	// June InitState 32b43d10 @32b43d52 ( CH1m calls it )

	bool				StateChangeBySkillID( const CX2SkillTree::SKILL_ID eSkillID_ );	// June 32aeaa10
	void				UpGageNowMpOfForm( const float fChangeOfNowMp_, const bool bCurrentForm_ );	// June 32aeb1e0
	int					GetNowCombinationPoint() const;	// June 32b0fa00
	void				UpCombinationPoint( const int iCombinationPoint_ );	// June 32b14860
	int					GetSkillCombinationPointCost( const CX2SkillTree::SKILL_ID eSkillID_, const int iSkillLevel_ );	// June 32b14f70
	void				KillMySummonedNPC( const CX2UnitManager::NPC_UNIT_ID eNPCUnitID_ );	// June 32b20100
	void				ResetPhantomRushReady();	// June 32b34920
	void				ClearAltarOfEvilEffectSet();	// June 32b35540
	void				ReleaseTransformedSkinAnim( const int iForm_ );	// char-luciel CH1a4: June 32b13d30
	void				SetFormSkinAnim( const int iForm_, STransformedSkin& transformedSkin_ );	// char-luciel CH1a4: June 32b13e20
	void				EndTransformedForm( const int iForm_, const bool bByDie_ );	// char-luciel CH1a4: June 32b38f30

protected:
	// June +0x959c .. +0x98a0 in June's order; ch1_offsets.tsv names the users ( a name marked provisional there is refined by its phase )
	CKTDGParticleSystem::CParticleEventSequenceHandle	m_ahLucielMajorParticleInstance[LUCIEL_MAJOR_PII_END];	// +0x959c
	CKTDGParticleSystem::CParticleEventSequenceHandle	m_ahLucielMinorParticleInstance[LUCIEL_MINOR_PII_END];	// +0x95a0
	float								m_fComboXXXZChargeTime;				// +0x95bc
	std::vector<CX2DamageEffect::CEffectHandle>	m_vecChaserOfDarknessEffect;	// +0x95c0
	float								m_fChaserOfDarknessFireTimer;		// +0x95d0
	float								m_fChaserOfDarknessPowerRate;		// +0x95d4
	bool								m_bOtherEquipNotRendered;			// +0x95d8
	USHORT								m_usStateLoopCount;					// +0x95da
	USHORT								m_usSoulStrikeLoopCount;			// +0x95dc
	int									m_iSnipingComboCount;				// +0x95e0
	int									m_iSnipingComboMaxCount;			// +0x95e4: June's ctor leaves it unset
	float								m_fChargeTime;						// +0x95e8
	float								m_fDeathCrisisOfMarbasWaitTime;		// +0x95ec
	bool								m_bDeadHandsActive;					// +0x95f0
	int									m_iSeizedByFearSkillLevel;			// +0x95f4
	float								m_fSeizedByFearRate;				// +0x95f8
	float								m_fSeizedByFearTimer;				// +0x95fc
	float								m_afSeizedByFearValue[2];			// +0x9600
	int									m_iSoulCollectorSkillLevel;			// +0x9608
	float								m_fPowerOfOriaxTime;				// +0x960c
	UCHAR								m_ucSoulCollectorCount;				// +0x9610
	float								m_fPowerOfOriaxDamageRate;			// +0x9614
	float								m_fSoulCollectorDamageRate;			// +0x9618
	USHORT								m_usSoulCollectorMaxCount;			// +0x961c
	float								m_fSoulExtortionDrainRate;			// +0x9620
	CX2EffectSet::Handle				m_hDemonicBreathChargeEffectSet;	// +0x9624
	float								m_fFantasieImpromptuWaitTime;		// +0x9628
	float								m_fDeathCrisisOfMarbasChargeTime;	// +0x962c
	float								m_fLeftHandRotateDegree;			// +0x9630
	float								m_fDeathCrisisOfMarbasMinChargeTime;	// +0x9634
	float								m_fDeathCrisisOfMarbasMaxChargeTime;	// +0x9638
	CX2EffectSet::Handle				m_hSoulShockEffectSet;				// +0x963c
	float								m_fCommunionRate;					// +0x9640
	float								m_afFormMpRegenerationRate[2];		// +0x9644: June's ctor leaves them unset
	float								m_fComboXXDownXXLoopTime;			// +0x964c
	CX2EffectSet::Handle				m_hDashComboDownXEffectSet;			// +0x9650
	D3DXVECTOR3							m_vDashComboDownXPosition;			// +0x9654
	USHORT								m_usDancingBladeLoopCount;			// +0x9660
	float								m_fSummonedDarknessElapsedTime;		// +0x9664
	int									m_iLordOfFearSkillLevel;			// +0x9668
	float								m_fReduceDebuffDurationRate;		// +0x966c
	float								m_fLordOfFearDamageRate;			// +0x9670
	float								m_fFearDamageEffectCoolTime;		// +0x9674
	bool								m_bAuraOfFearActive;				// +0x9678
	float								m_fAuraOfFearTime;					// +0x967c
	float								m_fAuraOfFearDuration;				// +0x9680
	float								m_fEnhancedSpecterRate;				// +0x9684
	float								m_fEnhancedSpecterRange;			// +0x9688
	float								m_fSoulStrikeChargeTime;			// +0x968c
	int									m_iShadowBoltControlIndex;			// +0x9690
	float								m_fShadowBoltTime;					// +0x9694
	float								m_fShadowBoltDuration;				// +0x9698
	bool								m_bTwilightJudgementReattack;		// +0x969c
	float								m_fTwilightJudgementTime;			// +0x96a0
	float								m_fSoulStrikeCoolTime;				// +0x96a4
	float								m_fCombinationPointOnHitCoolTime;	// +0x96a8
	CX2EffectSet::Handle				m_hDarkCrescentEffectSet;			// +0x96ac
	int									m_iEvilClawHitCount;				// +0x96b0
	float								m_fSwitchingCoolTime;				// +0x96b4
	int									m_iSpecialAttackCombinationPointCost;	// +0x96b8
	int									m_iLastUsedSkillID;					// +0x96bc
	float								m_fSoulStrikeMoveSpeed;				// +0x96c0
	float								m_fSoulStrikeSpeedRate;				// +0x96c4
	std::vector<SGameUnitKey>			m_vecDarkCrescentTargetUnit;		// +0x96c8
	int									m_iBoundingStrikeCount;				// +0x96dc: June's ctor leaves it unset
	bool								m_bBoundingStrikeHit;				// +0x96e0: June's ctor leaves it unset
	bool								m_bBoundingTargetFound;				// +0x96e8
	int									m_iBoundingTargetUnitIndex;			// +0x96ec
	SGameUnitKey						m_BoundingTarget;					// +0x96f0
	float								m_fBoundingKeyShowTime;				// +0x9700
	CKTDGUIDialogType					m_pDlgBoundingKey;					// +0x9704
	bool								m_bPhantomRushReady;				// +0x9710
	float								m_fPhantomRushReadyTime;			// +0x9714
	CX2EffectSet::Handle				m_hPhantomRushEffectSet;			// +0x9718
	SGameUnitKey						m_PhantomRushTarget;				// +0x9720
	KProtectedType<float>				m_fBoundingDamageRate;				// +0x9730
	KProtectedType<float>				m_fBoundingDamageRateEx;			// +0x973c
	SAltarOfEvilData					m_AltarOfEvilData;					// +0x9748
	bool								m_bNightmareTargetHit;				// +0x9778: June's ctor leaves it unset
	std::vector<CX2GameUnit*>			m_vecNightmareTargetUnit;			// +0x977c
	std::vector<SGameUnitKey>			m_vecNightmareHitUnit;				// +0x978c
	float								m_fDeathTouchTime;					// +0x97a0
	float								m_fDeathTouchRange;					// +0x97a4
	SGameUnitKey						m_DeathTouchTarget;					// +0x97a8
	UidType								m_uidDeathTouchUnit;				// +0x97b8
	float								m_fRapidImpactCoolTime;				// +0x97c0
	float								m_fRapidImpactRange;				// +0x97c4
	float								m_fRapidImpactMaxRange;				// +0x97c8
	float								m_fInvasionOfDarkEnergyTime;		// +0x97cc
	float								m_fInvasionOfDarkEnergyDuration;	// +0x97d0
	std::vector<STimedGameUnitKey>		m_vecInvasionOfDarkEnergyTarget;	// +0x97d4
	int									m_iDesperadoAttackCount;			// +0x97e4
	int									m_iDesperadoMaxAttackCount;			// +0x97e8
	std::vector<STimedGameUnitKey>		m_vecDesperadoTarget;				// +0x97ec
	float								m_fDesperadoAimDegreeX;				// +0x97fc: June's ctor leaves it unset
	float								m_fDesperadoAimDegreeY;				// +0x9800: June's ctor leaves it unset
	float								m_fDesperadoAttackTime;				// +0x9804: June's ctor leaves it unset
	float								m_fMadnessCraveAttackRate;			// +0x9808
	float								m_fMadnessCraveMagicAttackRate;		// +0x980c
	float								m_fMadnessCraveDefenceRate;			// +0x9810
	float								m_fMadnessCraveMagicDefenceRate;	// +0x9814
	float								m_fMadnessCraveHpRate;				// +0x9818
	float								m_fMadnessCraveCoolTime;			// +0x981c
	float								m_fMadnessCraveElapsedTime;			// +0x9820
	float								m_fMadnessCraveDuration;			// +0x9824
	float								m_fMadnessDestroyRate;				// +0x9828
	float								m_fMadnessDestroyCoolTime;			// +0x982c
	CX2GUAra::sLockOnSystemPtr			m_ptrShrewdCutterLockOn;			// +0x9830: Ara's type until CH3c20 moves it out of CX2GUAra
	int									m_iShrewdCutterAttackCount;			// +0x9834
	float								m_fShadowFuryAccumulatedDamage;		// +0x9838: June's ctor leaves it unset
	CX2EffectSet::Handle				m_hTransformedFormEffectSet;		// +0x983c
	CX2EffectSet::Handle				m_hPunisherEffectSet;				// +0x9840
	clock_t								m_clockCreated;						// +0x9844
	std::map<int, bool>					m_mapScaleHeadByForm;				// +0x9848
	int									m_iSoulSpreadCount;					// +0x9858
	float								m_fThroneAuthorityRate;				// +0x985c
	bool								m_bStableDarkEnergyLocked;			// +0x9860
	D3DXVECTOR3							m_vAbyssSnipingAimPosition;			// +0x9864
	D3DXVECTOR2							m_vAbyssSnipingAimRange;			// +0x9870
	std::vector<int>					m_vecAbyssSnipingTarget;			// +0x9878
	float								m_fCombinationPointOverflowCoolTime;	// +0x9888
	int									m_iDamageEffectIndex;				// +0x988c: June's ctor leaves it unset
	std::vector<SDamageEffectHitCount>	m_vecDamageEffectHitCount;			// +0x9890
};

#endif SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE
//}}
