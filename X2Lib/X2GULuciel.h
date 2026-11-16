//{{ Iruha : 2026-11-14 // char-luciel CH1a: Lu/Ciel's class ( June 32ae4080-32b4e76f, vtable 334f06fc, 0x98a0 bytes )
#ifdef SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE
#pragma once
#include "./X2GUAra.h"

class CX2GULuciel : public CX2GUUser
{
public:

	// june-state-ids S2: June state ids ( char-luciel CH0 ch0_reg.tsv / ch0_state_skill.tsv ); 7-130 equal CX2GUUser's USI_ ( June's LSI_ names kept )
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
		// june-state-ids S2: June's slot states 50-69 are CX2GUUser's USI_SPECIAL_ATTACK_* ( the CH1n shadow copies are dropped )
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
		LUCIEL_MINOR_PII_REVENGE_WAIT,			// char-luciel CH1l: June 32b37780 names the seven handles of +0x95a0
		LUCIEL_MINOR_PII_GROUND_SHOCK_WAVE_BLUE,
		LUCIEL_MINOR_PII_IMPACT_TICK_BLUE,
		LUCIEL_MINOR_PII_KU_AANG,
		LUCIEL_MINOR_PII_GROUND_SHOCK_WAVE_RED,
		LUCIEL_MINOR_PII_IMPACT_TICK_RED,
		LUCIEL_MINOR_PII_MAGNUM_BLADE_FIRE_WAVE,
		LUCIEL_MINOR_PII_END,
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

	// char-luciel CH1l: June 8-byte entry of 32b176a0: a worn equip's body byte ( CX2Eqip +0x15d4 ) and item id ( +0x1a0 )
	struct SFormEquipItem
	{
		bool							m_bMinorUnitEquip;
		int								m_iItemID;
	};

	// June +0x9748, member initializer 32b4d1c0, destructor 32699670 ( Altar of Evil, CH2d2 )
	struct SAltarOfEvilData
	{
		float								m_fMpCostPerEffectSet;	// char-luciel CH1l3: June 32b35600 pays it per Altar of Evil effect set ( CH1a named it m_fDuration )
		float								m_fRange;
		float								m_fInterval;
		float								m_fElapsedTime;
		std::vector<CX2EffectSet::Handle>	m_vecEffectSet;
		std::vector<D3DXVECTOR3>			m_vecPosition;

		SAltarOfEvilData() : m_fMpCostPerEffectSet( 2.f ), m_fRange( 1200.f ), m_fInterval( 0.2f ), m_fElapsedTime( 0.f ) {}
	};

	virtual void		CountDamageEffectHit( const CX2DamageManager::DamageData* pDamageData_ );	// own virtual 0x880, June 32b14db0
	virtual bool		TryUseSkillWithCombinationPoint( const CX2SkillTree::SKILL_ID eSkillID_,
							const CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseCondition_ );	// own virtual 0x884, June 32b3acf0; char-luciel CH2b: June answers whether the skill started ( al )
	virtual void		PreloadAuraAccelResources();	// own virtual 0x888, June 32ae6130

	// char-luciel CH1k: overrides band 1 part 1 ( June 32ae4080 - 32b1e8e0 ); bases in X2GUUser.h / X2GameUnit.h
	virtual void		Init( bool bUseTeamPos_, int iStartPosIndex_ );	// June 32b1e8e0 ( vslot 0x40 )
	virtual void		InitComponent();	// June 32b0f080 ( vslot 0x3c )
	virtual void		InitEffect();	// June 32ae4f30 ( vslot 0x48 )
	virtual void		DamageReact( CX2DamageManager::DamageData* pDamageData_ );	// June 32ae4080 ( vslot 0x60 )
	virtual void		Win();	// June 32aea610 ( vslot 0x90 )
	virtual void		Lose();	// June 32b0eff0 ( vslot 0x94 )
	virtual void		Draw();	// June 32aea650 ( vslot 0x98 )
	virtual float		GetNowMp() const;	// June 32aeb0c0 ( vslot 0xd8 )
	virtual float		GetMaxMp() const;	// June 32aeb0f0 ( vslot 0xdc )
	virtual void		SetMaxMp( const float fMaxMp_ );	// June 32aeb120 ( vslot 0xe0 )
	virtual bool		FlushMp( const float fFlushMp_, const int iMpGageIndex_ = 0 );	// June 32aeb280 ( vslot 0xe4 )
	virtual void		UpNowMp( const float fChangeOfNowMp_ );	// June 32aeb160 ( vslot 0xf0 )
	virtual void		SetNowMp( float fNowMp_, bool bTrackMp_ = false );	// June 32aeb240 ( vslot 0xf8 )
	virtual void		StampEffectSetIndex( CX2EffectSet::EffectSetInstance* pEffectSetInstance_ );	// June 32b0efc0 ( vslot 0x248 )
	virtual int			GetWeaponIndexHavingModel( const int iModelIndex_ ) const;	// June 32b151b0 ( vslot 0x2b0 )
	virtual void		SetDamageExceptionProcess( CX2DamageManager::DamageData* pDamageData_, float& fPowerRate_ );	// June 32aea530 ( vslot 0x2b8 )
	virtual float		GetReduceDebuffDurationRate( BUFF_TEMPLET_ID eBuffTempletID_ );	// June 32ae5480 ( vslot 0x29c )
	virtual int			GetDamageEffectIndex() const;	// June 32b0efe0 ( vslot 0x2e0 )
	virtual void		DoProcessWhenDamageDataReset( const CX2DamageManager::DamageData* pDamageData_ );	// June 32b17660 ( vslot 0x2e4 ), base: CH1o
	virtual void		DoProcessWhenDamageDataDestroyed( const CX2DamageManager::DamageData* pDamageData_ );	// June 32b17680 ( vslot 0x2fc ), base: char-ara CH1o
	virtual void		AdjustDamageDataBeforeDamageReact( CX2DamageManager::DamageData* pDamageData_ );	// June 32b1e720 ( vslot 0x300 ), base: char-raven CH1o
	virtual void		DoProcessWhenKillEnemy( const CX2GameUnit* pDefender_ );	// June 32b0e3f0 ( vslot 0x318 )
	virtual void		ChangeMaxCharacterAddResource( const float fRate_ );	// June 32b14cd0 ( vslot 0x324 )

	// char-luciel CH1k2: overrides band 1 part 2 ( June 32b1f0a0 - 32b4db90 )
	virtual void		DeleteMinorParticle();	// June 32b1f0a0 ( vslot 0x4c )
	virtual void		HyperModeBuffEffectStart();	// June 32b1f0b0 ( vslot 0xc4 )
	virtual void		ReInit( bool bUseTeamPos_ = true, int iStartPosIndex_ = -1 );	// June 32b373e0 ( vslot 0x44 )
	virtual CX2GageData*	CreateGageData();	// June 32b37c70 ( vslot 0xd0 )
	virtual bool		StateChange( int iState_, bool bDirectChange_ = false );	// June 32b4db90 ( vslot 0x5c ): the state id goes through GetStateIDByForm
	virtual bool		StateChange( int iState_, int iSubState_, bool bDirectChange_ = false );	// June 32b4dbb0 ( vslot 0x434 )
	virtual int			GetWaitStateID();	// June 32b4d3c0 ( vslot 0x438 ) ( base: char-luciel CH1o )
	virtual int			GetWalkStateID();	// June 32b4d3f0 ( vslot 0x43c ) ( base: char-luciel CH1o )
	virtual int			GetDashStateID();	// June 32b4d430 ( vslot 0x440 ) ( base: char-luciel CH1o )
	virtual int			GetDashEndStateID();	// June 32b4d470 ( vslot 0x444 ) ( base: char-luciel CH1o )
	virtual int			GetDashJumpStateID();	// June 32b4d4b0 ( vslot 0x448 ) ( base: char-luciel CH1o )
	virtual int			GetDashJumpLandingStateID();	// June 32b4d4f0 ( vslot 0x44c ) ( base: char-luciel CH1o )
	virtual int			GetJumpReadyStateID();	// June 32b4d530 ( vslot 0x450 ) ( base: char-luciel CH1o )
	virtual int			GetJumpUpStateID();	// June 32b4d570 ( vslot 0x454 ) ( base: char-luciel CH1o )
	virtual int			GetJumpLandingStateID();	// June 32b4d610 ( vslot 0x45c ) ( base: char-luciel CH1o )
	virtual int			GetDieFrontStateID() const;	// char-luciel CH1o2 const: June 32b4d650 ( vslot 0x460 ) ( base: char-luciel CH1o )
	virtual int			GetDieBackStateID() const;	// char-luciel CH1o2 const: June 32b4d690 ( vslot 0x464 ) ( base: char-luciel CH1o )
	virtual int			GetDamageGroggyStateID() const;	// June 32b4d6d0 ( vslot 0x468 ), base: DU2
	virtual int			GetDamageSmallFrontStateID() const;	// June 32b4d710 ( vslot 0x46c )
	virtual int			GetDamageSmallBackStateID() const;	// June 32b4d750 ( vslot 0x470 )
	virtual int			GetDamageBigFrontStateID() const;	// June 32b4d790 ( vslot 0x474 )
	virtual int			GetDamageBigBackStateID() const;	// June 32b4d7d0 ( vslot 0x478 )
	virtual int			GetDamageStandUpFrontStateID() const;	// June 32b4d890 ( vslot 0x47c )
	virtual int			GetDamageStandUpBackStateID() const;	// June 32b4d8d0 ( vslot 0x480 )
	virtual int			GetDamageAirSmallStateID() const;	// June 32b4d910 ( vslot 0x484 )
	virtual int			GetDamageAirDownStateID() const;	// June 32b4d950 ( vslot 0x488 )
	virtual int			GetDamageAirUpStateID() const;	// June 32b4da50 ( vslot 0x498 )

	// char-luciel CH1l: overrides band 2 part 1 ( June 32ae9bf0 - 32b4dbe0 )
	virtual bool		CanPlayEmotion( const CX2Unit::EMOTION_TYPE eEmotionType_ );	// June 32ae9bf0 ( vslot 0x574 ), base 32ec5990 returns true ( base: char-luciel CH1o )
	virtual int			GetTransformedSkillStateID( const CX2SkillTree::SKILL_ID eSkillID_ );	// June 32ae9c00 ( vslot 0x57c ), base: CH1a0
	virtual bool		IsTransformSkillState();	// June 32ae9de0 ( vslot 0x580 ), base 31e4c550 returns false; no March engine caller: CommonStateStart @32061976 stores it in +0x94c0, which nothing reads
	virtual void		SetShowViewListAndUpdate( const bool bShow_ );	// June 32ae9f20 ( vslot 0x640 ), base 31fca010: SetShowViewList( b ), then UpdateNotRenderingOtherEquipList() when b ( base: char-luciel CH1o )
	virtual void		InitEnchantWeaponEffectUnitSpecific();	// June 32aea690 ( vslot 0x54c )
	virtual void		SetNowMpFromStoredValue();	// June 32aeb2c0 ( vslot 0x650 ), base 31fceed0 ( base: char-luciel CH1o )
	virtual void		DoProcessWhenActiveBodyChangeApplied( const int iDuoType_, const int iCpAmount_ );	// June 32b0fa20 ( vslot 0x648 ), base 33093ad0 is empty ( base: char-luciel CH1o )
	virtual void		COMMON_BUFF_FrameMove();	// June 32b12fd0 ( vslot 0x5f0 )
	virtual CKTDGXSkinAnimPtr	GetSkinAnimByForm( const int iForm_ );	// June 32b13b60 ( vslot 0x570 ), base 31fd48a0 ( base: char-luciel CH1o )
	virtual void		SetHandEquipShow( const bool bShow_ );	// June 32b143e0 ( vslot 0x62c ), base 3208ddc0 is empty ( base: char-luciel CH1o )
	virtual bool		CanRidingState();	// June 32b14e10 ( vslot 0x628 )
	virtual bool		IsClassCommonMoveState();	// June 32b15340 ( vslot 0x578 )
	virtual void		CreateMajorParticleArray();	// June 32b154f0 ( vslot 0x540 )
	virtual void		GetSkillIconTexture( CKTDGUISlot* pSlot_, const CX2SkillTree::SkillTemplet* pSkillTemplet_ );	// June 32b15900 ( vslot 0x504 ), base 31fdb990; no caller in June's exe
	virtual void		SkillIconOverride( const int iSkillID_, wstring& wstrIconTexName_, wstring& wstrIconTexKey_ );	// June 32b16220 ( vslot 0x56c )
	virtual void		UpdateNotRenderingOtherEquipList();	// June 32b18220 ( vslot 0x630 )
	virtual void		SyncFutureMpFromGage();	// June 32b1c960 ( vslot 0x64c ), base 31fe9da0 ( base: char-luciel CH1o )
	virtual bool		CheckBoundingTarget();	// June 32b34680 ( vslot 0x568 ), base 31e4c550 returns false; no March engine caller: the bounding states call it
	virtual void		AttackResultByType( CX2DamageManager::DamageData& pDamageData_ );	// June 32b37730 ( vslot 0x4e0 )
	virtual void		CreateMinorParticleArray();	// June 32b37780 ( vslot 0x548 )
	virtual void		HyperModeFrameMove();	// June 32b38080 ( vslot 0x600 )
	virtual int			GetJumpDownStateID();	// June 32b4d5b0 ( vslot 0x458 ), not const as the base
	virtual int			GetDamageDownLandFront() const;	// June 32b4d810 ( vslot 0x114 ), DU2's name
	virtual int			GetDamageDownLandBack() const;	// June 32b4d850 ( vslot 0x118 )
	virtual int			GetDamageAirDownInvincibleStateID() const;	// June 32b4d990 ( vslot 0x48c )
	virtual int			GetDamageAirDownLanding() const;	// June 32b4d9d0 ( vslot 0x490 )
	virtual int			GetDamageAirFallStateID() const;	// June 32b4da10 ( vslot 0x494 )
	virtual int			GetDamageAirFlyFrontStateID() const;	// June 32b4da90 ( vslot 0x49c )
	virtual int			GetDamageAirFlyBackStateID() const;	// June 32b4dad0 ( vslot 0x4a0 )
	virtual int			GetDamageRevengeStateID() const;	// June 32b4db10 ( vslot 0x4a4 )
	virtual int			GetDanceWaitStateID();	// June 32b4db50 ( vslot 0x5c4 ), base 3208e140 returns the dance wait state; no March engine caller: June 323c5310 is the dance system ( Stage 10 )
	virtual bool		IsHyperModeState() const;	// June 32b4dbe0 ( vslot 0x51c )
	virtual void		SetSDOnePieceAvatar( const int iForm_, const bool bSDOnePieceAvatar_ );	// June 32b172b0 ( vslot 0x87c ), base 3208e280 stores m_bSDOnePieceAvatar; no March engine caller: the class calls it

	// char-luciel CH1l2: overrides band 2 part 2 ( June 32ae40c0 - 32b3a310 )
	virtual void		SetEquippedSkillLevel( const CX2SkillTree::SKILL_ID eSkillID_, const bool bChangeAll_ );	// June 32ae40c0 ( vslot 0x7b8 )
	virtual void		CommonStateStartFuture();	// June 32ae4f60 ( vslot 0x7c0 ): a jump to the base
	virtual void		CommonStateEndFuture();	// June 32ae4f70 ( vslot 0x7d8 ): a jump to the base
	virtual void		CommonStateEnd();	// June 32ae4f80 ( vslot 0x7dc ): a jump to the base
	virtual RENDER_HINT	CommonRender_Prepare();	// June 32ae4f90 ( vslot 0x7e8 ): the base
	virtual bool		SpecialAttackEventProcess( CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseCondition_ = CX2SkillTree::ASUT_GROUND );	// June 32ae4fb0 ( vslot 0x7f4 )
	virtual void		StateChangeDashIfPossible();	// June 32ae5400 ( vslot 0x668 ), base 31fc85b0 ( base: char-luciel CH1o )
	virtual bool		IsSkillChainReady( const int iSkillID_, const int iUnused_ );	// June 32ae9bd0 ( vslot 0x658 ), base 330fb7d0 is a stub: the Phantom Rush chain is ready; no March engine caller: June 322cc480 forwards a float second argument
	virtual float		ShownSkillCoolTime( const float fCoolTime_, const int iSkillID_ );	// June 32aea8c0 ( vslot 0x6a8 )
	virtual void		StoreGageSnapshot();	// June 32aeb410 ( vslot 0x740 ), base 32009780; no March engine caller: Crest of Time ( char-add )
	virtual void		RestoreGageSnapshot();	// June 32aeb430 ( vslot 0x744 ), base 3204da60; no March engine caller: Crest of Time ( char-add )
	virtual void		UpdateAfterImageEquip( const AFTER_IMAGE_BODY eBody_ = AIB_FIRST_UNIT );	// June 32b0e440 ( vslot 0x6b0 )
	virtual void		SumSocketOptionAfterSlots( CX2SocketItem::SocketData& kAggregate_, const CX2SocketItem::SocketData& kSocketData_ );	// June 32b0e670 ( vslot 0x794 )
	virtual void		SpecialAttackUse( const int iSkillID_, const bool bUnused_ );	// June 32b0f940 ( vslot 0x7fc ); class-own: each class calls its own
	virtual void		ResetMaxMP();	// June 32b0fa90 ( vslot 0x664 ), base 31fd2810 ( base: char-luciel CH1o )
	virtual float		GetActualMPConsume( const CX2SkillTree::SKILL_ID eSkillID_, const int iSkillLevel_ ) const;	// June 32b14640 ( vslot 0x6a0 )
	virtual float		GetActualCoolTime( IN const CX2SkillTree::SkillTemplet* pSkillTemplet_, IN int iSkillLevel_ ) const;	// June 32b15050 ( vslot 0x6a4 )
	virtual bool		IsNotCommonMoveState();	// June 32b15220 ( vslot 0x828 )
	virtual void		AfterImageFrameMove();	// June 32b15430 ( vslot 0x850 )
	virtual void		ShowActiveSkillCutInAndLightByScript( float fTimeToShow_, bool bOnlyLight_ );	// June 32b16c80 ( vslot 0x834 )
	virtual void		ScaleHead( float fHeadScale_, float fFaceScale_ );	// June 32b171c0 ( vslot 0x66c )
	virtual void		RegisterDamageEffectHitCountEntry();	// June 32b18170 ( vslot 0x848 ), base 31f3f070 is empty ( base: char-luciel CH1o )
	virtual bool		SpecialAttackNoStageChange( const CX2SkillTree::SkillTemplet* pSkillTemplet_ );	// June 32b1c880 ( vslot 0x810 )
	virtual void		CommonFrameMoveFuture();	// June 32b1edd0 ( vslot 0x7c8 )
	virtual void		SpecialAttackCooldown( const int iSkillID_, const float fMpConsume_ );	// June 32b1eef0 ( vslot 0x800 ); class-own: the engine reaches it through ReservedSkillStart ( CH1o3 )
	virtual void		ReservedSkillStart( const CX2SkillTree::SkillTemplet* pSkillTemplet_ );	// char-luciel CH1o3: June 32057b80 @32057c4c-cb2, vslots 0x800 / 0x764
	virtual void		CommonStateStart();	// June 32b37910 ( vslot 0x7c4 )
	virtual void		NoStateChangeActionFrameMove();	// June 32b37c40 ( vslot 0x80c )
	virtual void		CommonFrameMove();	// June 32b397a0 ( vslot 0x7cc )
	virtual bool		CanUseSpecialAttack( CX2SkillTree::ACTIVE_SKILL_USE_CONDITION eActiveSkillUseCondition_ = CX2SkillTree::ASUT_GROUND );	// June 32b3a310 ( vslot 0x7f8 ); class-own: each class calls its own

	void				SetBoundingTarget( const CX2GameUnit::GAME_UNIT_TYPE eTargetType_, const UidType uidTarget_, const int iTargetLastHitAttackSerial_ );	// char-luciel CH1p: June 32b38cb0, called by the bounding hooks of CX2GUUser 31fd02d0 and CX2GUNPC 3275a070

#ifdef SERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE
	// char-luciel CH1: June 3230f470, the factory's case 10 in 322ced00 / 322d5bf0 ( Chung's CreateGUChung pattern )
	static CX2GULuciel*	CreateGULuciel( int iUnitIndex_, int iTeamNum_,
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			lua_State*   pScriptLuaState,
#else	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			const WCHAR* pScriptFileName,
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		int iFrameBufferNum_, IN CX2Unit* pUnit_ )
	{
		return new CX2GULuciel( iUnitIndex_, iTeamNum_,
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			pScriptLuaState,
#else	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			pScriptFileName,
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
			iFrameBufferNum_, pUnit_ );
	}
#endif SERV_IRUHADEV_JUNE_CHAR_LUCIEL_LIVE
#ifdef SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SUMMON
	void				SetShadowBoltNpcUID( const int iShadowBoltNpcUID_ ) { m_iShadowBoltNpcUID = iShadowBoltNpcUID_; }	// char-luciel CH3a0: June 3230c1a0 0x774 writes +0x9690
#endif SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SUMMON

private:
	CX2GULuciel( int iUnitIndex_, int iTeamNum_,
#ifdef	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		lua_State*   pScriptLuaState,
#else	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		const WCHAR* pScriptFileName,
#endif	X2OPTIMIZE_GAME_CHARACTER_BACKGROUND_LOAD
		int iFrameBufferNum_, IN CX2Unit* pUnit_ );
	virtual ~CX2GULuciel();

#ifdef SERV_IRUHADEV_JUNE_CHAR_LUCIEL_UI
	friend class CX2Game;	// char-luciel CH5: June's UpdateSkillCoolTime reads the combination point cost and gauge ( 32b14f70 / 32b0fa00 )
#endif SERV_IRUHADEV_JUNE_CHAR_LUCIEL_UI

protected:
	void				InitializeLucielMajorParticleArray();
	void				InitializeLucielMinorParticleArray();
	void				DeleteLucielMinorParticle();	// June 32b1eb10
	void				SetSkillCutInSetIndexByUnitClass();	// June InitState 32b43d10 @32b43d52 ( CH1m calls it )

	bool				StateChangeBySkillID( const CX2SkillTree::SKILL_ID eSkillID_ );	// June 32aeaa10
	void				UpGageNowMpOfForm( const float fChangeOfNowMp_, const bool bCurrentForm_ );	// June 32aeb1e0
	int					GetNowCombinationPoint() const;	// June 32b0fa00
	void				UpCombinationPoint( const int iCombinationPoint_ );	// June 32b14860
	void				AddCombinationPointOnHit( const CX2DamageManager::DamageData& damageData_ );	// char-luciel CH1p: June 32b14bc0 ( thunk 32b4cee0 ), CH4a's InitPassiveSkillState pushes it
	int					GetSkillCombinationPointCost( const CX2SkillTree::SKILL_ID eSkillID_, const int iSkillLevel_ );	// June 32b14f70
	void				KillMySummonedNPC( const CX2UnitManager::NPC_UNIT_ID eNPCUnitID_ );	// June 32b20100
	// char-luciel CH1k: helpers of the overrides
	void				SetGageNowMp( const float fNowMp_, const bool bTrackMp_, const int iMpGageIndex_ );	// June 32ae50f0
	void				SetGageNowMpOfForm( const float fNowMp_, const bool bTrackMp_, const bool bCurrentForm_ );	// June 32b0e620
	void				SetNowCombinationPoint( const int iCombinationPoint_ );	// June 32aeb090
	int					GetStateIDByForm( const int iStateID_ );	// June 32aeb450
	void				LoadTransformedSkinAnim( const int iForm_ );	// June 32b162e0
	void				EraseDamageEffectIndexEntry( const CX2DamageManager::DamageData* pDamageData_ );	// June 32b16810
	bool				IsInvasionOfDarkEnergyTarget( const CX2GameUnit* pGameUnit_ );	// June 32b16260
	void				EraseInvasionOfDarkEnergyTarget( const CX2GameUnit* pGameUnit_ );	// June 32b166e0
	bool				IsInvasionOfDarkEnergyActive() const;	// June 32b4b580
	void				ResetPhantomRushReady();	// June 32b34920
	void				ClearAltarOfEvilEffectSet();	// June 32b35540
	void				PlayAltarOfEvilEffectSet();	// char-luciel CH1l3: June 32b35600
	void				ClearDeathTouch();	// char-luciel CH1l3: June 32b349f0
	void				ReleaseTransformedSkinAnim( const int iForm_ );	// char-luciel CH1a4: June 32b13d30
	void				SetFormSkinAnim( const int iForm_, STransformedSkin& transformedSkin_ );	// char-luciel CH1a4: June 32b13e20
	void				EndTransformedForm( const int iForm_, const bool bByDie_ );	// char-luciel CH1a4: June 32b38f30
	bool				IsInJobLine( int iJobLine_ );	// June 32aeb380 ( vslot 0x670 )
	// char-luciel CH1l2: CX2GUUser's skill state helpers, as Eve and Ara carry them
	void				SetSkillLevelStateData( const CX2SkillTree::SkillTemplet* pSkillTemplet_, UserUnitStateData& stateData_ );	// June 3201f850
	void				SetSkillLevelStateDataByID( const CX2SkillTree::SkillTemplet* pSkillTemplet_, const int iStateID_ );	// June 32028190 ( vslot 0x7a8 )
	// char-luciel CH1l2: class helpers the band 2 overrides call, bodies in CH1l3
	void				UpGageNowMp( const float fChangeOfNowMp_, const int iMpForm_ );	// June 32aeb1a0
	void				ShowActiveSkillCutInAndLight( const float fTimeToShow_, const int iSkillCutInSetSubIndex_, const bool bOnlyLight_ );	// June 32b16940
	void				CancelSummonDarkness();	// June 32b204d0
	bool				IsOpenMindConditionMet() const;	// June 32b4c7f0
	int					GetOpenMindSkillID() const;	// June 32b0ef80
	void				UpdateOpenMindBuff();	// June 32b14d10
	void				UpdateStableDarkEnergyBuff();	// June 32b0eed0
	void				FrameMoveChaserOfDarknessEffect();	// June 32b1f650
	void				FrameMoveShadowBoltSpear();	// June 32b20780
	void				FrameMoveTwilightJudgementMp();	// June 32aebb60
	void				FrameMoveAuraOfFear();	// June 32b15780
	void				FrameMoveDeathTouchEffect();	// June 32b34a90
	void				UpdateInvasionOfDarkEnergyTargetTime();	// June 32b16760
	void				FrameMoveAltarOfEvilEnd();	// June 32b35c60
	void				FrameMoveBoundingKeyDialog();	// June 32b0e550
	void				UpdateBoundingKeyDialogPosition();	// char-luciel CH1p: June 32b35cf0
	void				DieFrontStart();	// char-luciel CH1b: June 31fd2d60 ( CX2GUUser's shared body )
	void				DieBackStart();	// char-luciel CH1b: June 31fd2e30 ( CX2GUUser's shared body )

	// char-luciel CH1m: registration, and the CX2GUUser bodies only Lu / Ciel register
	virtual void		InitState();	// June 32b43d10 ( vslot 0x7a4 )
	void				InitStateCommon();	// June 32b3e8b0
	virtual void		ParseCommonRandomState();	// char-luciel CH1: March's pure virtual; June parses in CX2GUUser::Init ( 31feaac0 ) by class name
	void				DieFrontStartFuture();	// June 31fcd0f0
	void				DieFrontFrameMove();	// June 320008f0
	void				DieFrontEventProcess();	// June 31fcd160
	void				DieBackStartFuture();	// June 31fcd1b0
	void				DieBackFrameMove();	// June 32000910
	void				DieBackEventProcess();	// June 31fcd220
	void				DashEndStartFuture();	// June 31fd2680

	// char-luciel CH1n: June 32b46580 ( vslot 0x7b4, uncarved )
	virtual void		SetEquippedSkillFuncBySkillSlotIndex( const CX2SkillTree::SKILL_ID eSkillID_, const int iSlotIndex_, const bool bSlotB_ );

	// char-luciel CH1b: Lu's common states ( June 32ae5540 - 32b3a270 )
	void				LSI_LU_JUMP_UP_EventProcess();	// June 32ae5540
	void				LSI_LU_DASH_JUMP_POWER_LANDING_StateStartFuture();	// June 32ae55e0
	void				LSI_LU_DASH_JUMP_POWER_LANDING_StateStart();	// June 32ae5610
	void				LSI_LU_STANDUP_ROLLING_FRONT_FRONT_StateEnd();	// June 32ae5630
	void				LSI_LU_STANDUP_ROLLING_FRONT_BACK_StateEnd();	// June 32ae5670
	void				LSI_LU_STANDUP_ROLLING_BACK_FRONT_StateEnd();	// June 32ae56b0
	void				LSI_LU_STANDUP_ROLLING_BACK_BACK_StateEnd();	// June 32ae56f0
	void				LSI_LU_STANDUP_ATTACK_FRONT_StateEnd();	// June 32ae5730
	void				LSI_LU_STANDUP_ATTACK_BACK_StateEnd();	// June 32ae5770
	void				LSI_LU_STANDUP_ATTACK_FRONT_NO_DOWN_StateEnd();	// June 32ae58a0
	void				LSI_LU_STANDUP_ATTACK_BACK_NO_DOWN_StateEnd();	// June 32ae58e0
	void				LSI_LU_SWITCHING_StateStartFuture();	// June 32ae5920
	void				LSI_LU_WAIT_StateStartFuture();	// June 32aebc10
	void				LSI_LU_JUMP_DOWN_EventProcess();	// June 32aebc60
	void				LSI_LU_DAMAGE_DOWN_FRONT_EventProcess();	// June 32aebd10 ( also the thunk 32aec050 )
	void				LSI_LU_DAMAGE_DOWN_BACK_EventProcess();	// June 32aebeb0
	void				LSI_LU_STANDUP_ROLLING_FRONT_FRONT_EventProcess();	// June 32aec060
	void				LSI_LU_STANDUP_ROLLING_FRONT_BACK_EventProcess();	// June 32aec0f0
	void				LSI_LU_STANDUP_ROLLING_BACK_FRONT_EventProcess();	// June 32aec180
	void				LSI_LU_STANDUP_ROLLING_BACK_BACK_EventProcess();	// June 32aec210
	void				LSI_LU_STANDUP_ATTACK_FRONT_EventProcess();	// June 32aec2a0
	void				LSI_LU_STANDUP_ATTACK_BACK_EventProcess();	// June 32aec330
	void				LSI_LU_STANDUP_ATTACK_FRONT_NO_DOWN_EventProcess();	// June 32af0150
	void				LSI_LU_STANDUP_ATTACK_BACK_NO_DOWN_EventProcess();	// June 32af01e0
	void				LSI_LU_JUMP_DOWN_NO_ATTACK_EventProcess();	// June 32af1150
	void				LSI_LU_WAIT_StateStart();	// June 32b0fd90
	void				LSI_LU_JUMP_READY_EventProcess();	// June 32b0fde0
	void				LSI_LU_DASH_END_FrameMove();	// June 32b0fef0
	void				LSI_LU_DASH_JUMP_EventProcess();	// June 32b0ff90
	void				LSI_LU_DAMAGE_DOWN_FRONT_FrameMove();	// June 32b10260
	void				LSI_LU_DAMAGE_DOWN_BACK_FrameMove();	// June 32b10310
	void				LSI_LU_JUMP_DOWN_NO_ATTACK_FrameMoveFuture();	// June 32b11670
	void				LSI_LU_WAIT_EventProcess();	// June 32b20b30
	void				LSI_LU_WALK_EventProcess();	// June 32b20d70
	void				LSI_LU_JUMP_LANDING_EventProcess();	// June 32b20f70
	void				LSI_LU_DASH_EventProcess();	// June 32b21250
	void				LSI_LU_DASH_END_EventProcess();	// June 32b214c0
	void				LSI_LU_DASH_JUMP_LANDING_EventProcess();	// June 32b21830
	void				LSI_LU_DASH_JUMP_POWER_LANDING_EventProcess();	// June 32b21a70
	void				LSI_LU_DIE_FRONT_StateStart();	// June 32b3a250
	void				LSI_LU_DIE_BACK_StateStart();	// June 32b3a270

	// char-luciel CH1d: Ciel's common states ( June 32ae5960 - 32b235c0 )
	void				LSI_CIEL_JUMP_UP_EventProcess();	// June 32ae5960
	void				LSI_CIEL_STANDUP_ROLLING_FRONT_FRONT_StateEnd();	// June 32ae5a00
	void				LSI_CIEL_STANDUP_ROLLING_FRONT_BACK_StateEnd();	// June 32ae5a40
	void				LSI_CIEL_STANDUP_ROLLING_BACK_FRONT_StateEnd();	// June 32ae5a80
	void				LSI_CIEL_STANDUP_ROLLING_BACK_BACK_StateEnd();	// June 32ae5ac0
	void				LSI_CIEL_STANDUP_ATTACK_FRONT_StateEnd();	// June 32ae5b00
	void				LSI_CIEL_STANDUP_ATTACK_BACK_StateEnd();	// June 32ae5b40
	void				LSI_CIEL_JUMP_DOWN_EventProcess();	// June 32af1b80
	void				LSI_CIEL_DAMAGE_DOWN_FRONT_EventProcess();	// June 32af1c30
	void				LSI_CIEL_DAMAGE_DOWN_BACK_EventProcess();	// June 32af1da0
	void				LSI_CIEL_STANDUP_ROLLING_FRONT_FRONT_EventProcess();	// June 32af1f20
	void				LSI_CIEL_STANDUP_ROLLING_FRONT_BACK_EventProcess();	// June 32af1fb0
	void				LSI_CIEL_STANDUP_ROLLING_BACK_FRONT_EventProcess();	// June 32af2040
	void				LSI_CIEL_STANDUP_ROLLING_BACK_BACK_EventProcess();	// June 32af20d0
	void				LSI_CIEL_STANDUP_ATTACK_FRONT_EventProcess();	// June 32af2160
	void				LSI_CIEL_STANDUP_ATTACK_BACK_EventProcess();	// June 32af21f0
	void				LSI_CIEL_JUMP_DOWN_NO_ATTACK_EventProcess();	// June 32af5cf0
	void				LSI_CIEL_JUMP_READY_EventProcess();	// June 32b11720
	void				LSI_CIEL_DASH_END_FrameMove();	// June 32b11830
	void				LSI_CIEL_DASH_JUMP_EventProcess();	// June 32b118d0
	void				LSI_CIEL_DAMAGE_DOWN_FRONT_FrameMove();	// June 32b11ba0
	void				LSI_CIEL_DAMAGE_DOWN_BACK_FrameMove();	// June 32b11c50
	void				LSI_CIEL_JUMP_DOWN_NO_ATTACK_FrameMoveFuture();	// June 32b12e20
	void				LSI_CIEL_WAIT_EventProcess();	// June 32b22710
	void				LSI_CIEL_WALK_EventProcess();	// June 32b22950
	void				LSI_CIEL_JUMP_LANDING_EventProcess();	// June 32b22b50
	void				LSI_CIEL_DASH_EventProcess();	// June 32b22e30
	void				LSI_CIEL_DASH_END_EventProcess();	// June 32b23050
	void				LSI_CIEL_DASH_JUMP_LANDING_EventProcess();	// June 32b23380
	void				LSI_CIEL_DASH_JUMP_POWER_LANDING_EventProcess();	// June 32b235c0

	// char-luciel CH1f: Duo combos part 1 ( June 32ae57b0 - 32af2cc0 )
	void				LSI_LU_COMBO_XXXZ_READY_StateStart();	// June 32ae57b0
	void				LSI_LU_COMBO_XXXZ_LOOP_EventProcess();	// June 32ae57d0
	void				LSI_LU_JUMP_COMBO_Z_FrameMoveFuture();	// June 32ae5810
	void				LSI_LU_JUMP_COMBO_X_FrameMoveFuture();	// June 32ae5850
	void				LSI_CIEL_COMBO_XXXZ_START_StateStart();	// June 32ae5b80
	void				LSI_CIEL_COMBO_XXXZ_LOOP_StateEnd();	// June 32ae5ba0
	void				LSI_CIEL_JUMP_COMBO_Z_FrameMoveFuture();	// June 32ae5be0
	void				LSI_CIEL_JUMP_COMBO_X_FrameMoveFuture();	// June 32ae5c20
	void				LSI_LU_JUMP_COMBO_Z_SECOND_FrameMoveFuture();	// June 32ae6050
	void				LSI_LU_COMBO_Z_EventProcess();	// June 32aec3c0
	void				LSI_LU_COMBO_ZZ_EventProcess();	// June 32aec6b0
	void				LSI_LU_COMBO_ZZZ_EventProcess();	// June 32aec9a0
	void				LSI_LU_COMBO_ZZZZ_EventProcess();	// June 32aece00
	void				LSI_LU_COMBO_ZZZZX_EventProcess();	// June 32aed0f0
	void				LSI_LU_COMBO_X_EventProcess();	// June 32aed320
	void				LSI_LU_COMBO_XX_EventProcess();	// June 32aed640
	void				LSI_LU_COMBO_XXX_EventProcess();	// June 32aed9f0
	void				LSI_LU_COMBO_XXXX_EventProcess();	// June 32aede70
	void				LSI_LU_COMBO_XXXZ_READY_EventProcess();	// June 32aee0a0
	void				LSI_LU_COMBO_XXXZ_CHARGE_EventProcess();	// June 32aee130
	void				LSI_LU_COMBO_XXXZ_END_EventProcess();	// June 32aee1b0
	void				LSI_LU_COMBO_XXXZX_EventProcess();	// June 32aee4a0
	void				LSI_LU_DASH_COMBO_Z_EventProcess();	// June 32aee6d0
	void				LSI_LU_DASH_COMBO_X_START_EventProcess();	// June 32aee9e0
	void				LSI_LU_DASH_COMBO_X_LANDING_EventProcess();	// June 32aeea40
	void				LSI_LU_JUMP_COMBO_X_EventProcess();	// June 32aeee30
	void				LSI_LU_DASH_JUMP_COMBO_Z_EventProcess();	// June 32aeee70
	void				LSI_LU_DASH_JUMP_COMBO_ZX_EventProcess();	// June 32aef050
	void				LSI_LU_DASH_JUMP_COMBO_X_EventProcess();	// June 32aef160
	void				LSI_LU_DASH_JUMP_COMBO_XX_START_FrameMove();	// June 32aef530
	void				LSI_LU_DASH_JUMP_COMBO_XX_START_EventProcess();	// June 32aef660
	void				LSI_LU_DASH_JUMP_COMBO_XX_END_EventProcess();	// June 32aef690
	void				LSI_LU_DASH_JUMP_COMBO_XXX_EventProcess();	// June 32aef960
	void				LSI_LU_DASH_JUMP_COMBO_XXXX_EventProcess();	// June 32aefc40
	void				LSI_LU_DASH_JUMP_COMBO_XXXXZ_EventProcess();	// June 32aeff50
	void				LSI_CIEL_COMBO_Z_EventProcess();	// June 32af2280
	void				LSI_CIEL_COMBO_ZZ_EventProcess();	// June 32af2570
	void				LSI_CIEL_COMBO_ZZZ_EventProcess();	// June 32af2860
	void				LSI_CIEL_COMBO_ZZZZ_EventProcess();	// June 32af2cc0

	// char-luciel CH1f2: Duo combos part 2 ( June 32af2fb0 - 32b25cd0 )
	void				LSI_CIEL_COMBO_ZZZZX_EventProcess();	// June 32af2fb0
	void				LSI_CIEL_COMBO_X_EventProcess();	// June 32af31e0
	void				LSI_CIEL_COMBO_XX_EventProcess();	// June 32af3500
	void				LSI_CIEL_COMBO_XXX_EventProcess();	// June 32af38b0
	void				LSI_CIEL_COMBO_XXXX_EventProcess();	// June 32af3d30
	void				LSI_CIEL_COMBO_XXXZ_START_EventProcess();	// June 32af3f60
	void				LSI_CIEL_COMBO_XXXZ_LOOP_EventProcess();	// June 32af3fe0
	void				LSI_CIEL_COMBO_XXXZ_END_EventProcess();	// June 32af41e0
	void				LSI_CIEL_COMBO_XXXZX_EventProcess();	// June 32af44d0
	void				LSI_CIEL_DASH_COMBO_Z_EventProcess();	// June 32af4700
	void				LSI_CIEL_DASH_COMBO_X_EventProcess();	// June 32af4b90
	void				LSI_CIEL_JUMP_COMBO_X_EventProcess();	// June 32af4f80
	void				LSI_CIEL_DASH_JUMP_COMBO_Z_EventProcess();	// June 32af4fc0
	void				LSI_CIEL_DASH_JUMP_COMBO_ZX_EventProcess();	// June 32af51b0
	void				LSI_CIEL_DASH_JUMP_COMBO_X_EventProcess();	// June 32af52c0
	void				LSI_CIEL_DASH_JUMP_COMBO_XX_EventProcess();	// June 32af5690
	void				LSI_CIEL_DASH_JUMP_COMBO_XXX_EventProcess();	// June 32af5790
	void				LSI_CIEL_DASH_JUMP_COMBO_XXXX_EventProcess();	// June 32af5890
	void				LSI_CIEL_DASH_JUMP_COMBO_XXXXZ_EventProcess();	// June 32af5990
	void				LSI_CIEL_DASH_JUMP_COMBO_XXXXZ_FrameMove();	// June 32af59c0
	void				LSI_CIEL_DASH_JUMP_COMBO_XXXXZ_LANDING_EventProcess();	// June 32af5af0
	void				LSI_LU_COMBO_XXXZ_CHARGE_StateEnd();	// June 32b103b0
	void				LSI_LU_JUMP_COMBO_Z_EventProcess();	// June 32b10540
	void				LSI_CIEL_JUMP_COMBO_Z_EventProcess();	// June 32b11cf0
	void				LSI_LU_JUMP_COMBO_Z_SECOND_EventProcess();	// June 32b12ed0
	void				LSI_LU_COMBO_XXXZ_END_FrameMove();	// June 32b21cd0
	void				LSI_LU_DASH_JUMP_COMBO_ZX_FrameMove();	// June 32b21f80
	void				LSI_CIEL_COMBO_X_FrameMove();	// June 32b23820
	void				LSI_CIEL_COMBO_XX_FrameMove();	// June 32b23b80
	void				LSI_CIEL_COMBO_XXX_FrameMove();	// June 32b23ee0
	void				LSI_CIEL_COMBO_XXXX_FrameMove();	// June 32b24560
	void				LSI_CIEL_COMBO_XXXZ_END_FrameMove();	// June 32b248c0
	void				LSI_CIEL_DASH_COMBO_X_FrameMove();	// June 32b24c40
	void				LSI_CIEL_JUMP_COMBO_X_FrameMove();	// June 32b24fa0
	void				LSI_CIEL_DASH_JUMP_COMBO_ZX_FrameMove();	// June 32b25300
	void				LSI_CIEL_DASH_JUMP_COMBO_XX_FrameMove();	// June 32b25610
	void				LSI_CIEL_DASH_JUMP_COMBO_XXX_FrameMove();	// June 32b25970
	void				LSI_CIEL_DASH_JUMP_COMBO_XXXX_FrameMove();	// June 32b25cd0
	void				PlayComboProjectileEffectSet( const WCHAR* pwszEffectSetName_, const float fConsumeMP_, const bool bAddMaxCountModifier_ );	// June 32b23820 and its siblings, inline in each

	// char-luciel CH1g: Chiliarch and Royal Guard combos ( June 32ae5c70 - 32af8f30 )
	void				LSI_LCG_COMBO_ZZZX_StateStartFuture();	// June 32ae5c70
	void				LSI_LCG_COMBO_SNIPING_Z_StateStartFuture();	// June 32ae5c90
	void				LSI_LCG_COMBO_SNIPING_X_CHARGE_StateStart();	// June 32ae5cb0
	void				LSI_LCG_COMBO_SNIPING_X_CHARGE_EventProcess();	// June 32ae5ce0
	void				LSI_LCG_COMBO_SNIPING_X_CHARGE_FrameMove();	// June 32ae5d20
	void				LSI_LCG_DASH_JUMP_COMBO_SNIPING_START_StateStartFuture();	// June 32ae5db0
	void				LSI_LCG_DASH_JUMP_COMBO_SNIPING_LOOP_StateStartFuture();	// June 32ae5dd0
	void				LSI_LCG_F_COMBO_ZZZX_StateStartFuture();	// June 32ae5e10
	void				LSI_LCG_F_COMBO_ZZZXX_LOOP_StateStartFuture();	// June 32ae5e30
	void				LSI_LCG_F_COMBO_ZZZXX_LOOP_FrameMove();	// June 32ae5e60
	void				LSI_LCG_F_COMBO_ZZZXX_LOOP_EventProcess();	// June 32ae5ef0
	void				LSI_LCG_F_COMBO_ZZZXZ_LOOP_StateStart();	// June 32ae5f40
	void				LSI_LCG_F_COMBO_XXXXX_LOOP_EventProcess();	// June 32ae5f60
	void				LSI_LCG_F_DASH_COMBO_ZXXX_LOOP_EventProcess();	// June 32ae5fb0
	void				LSI_LCG_F_DASH_COMBO_XXXX_LOOP_EventProcess();	// June 32ae6000
	void				LSI_LCG_F_DASH_JUMP_COMBO_XZZ_READY_StateStartFuture();	// June 32ae6090
	void				LSI_LCG_F_DASH_JUMP_COMBO_XZZ_LOOP_StateStartFuture();	// June 32ae60b0
	void				LSI_LLC_COMBO_ZZZX_EventProcess();	// June 32af0270
	void				LSI_LLC_DASH_COMBO_Z_EventProcess();	// June 32af0570
	void				LSI_LLC_DASH_COMBO_ZX_EventProcess();	// June 32af0860
	void				LSI_LLC_DASH_COMBO_ZXX_EventProcess();	// June 32af0b50
	void				LSI_LLC_DASH_COMBO_XX_EventProcess();	// June 32af0e50
	void				LSI_LCG_DASH_COMBO_ZX_EventProcess();	// June 32af5d20
	void				LSI_LCG_COMBO_ZZZX_EventProcess();	// June 32af6110
	void				LSI_LCG_COMBO_SNIPING_Z_EventProcess();	// June 32af6330
	void				LSI_LCG_COMBO_SNIPING_Z_FINISH_EventProcess();	// June 32af6560
	void				LSI_LCG_COMBO_SNIPING_X_FIRE_EventProcess();	// June 32af6780
	void				LSI_LCG_DASH_JUMP_COMBO_SNIPING_START_EventProcess();	// June 32af69b0
	void				LSI_LCG_DASH_JUMP_COMBO_SNIPING_LOOP_EventProcess();	// June 32af6a20
	void				LSI_LCG_DASH_JUMP_COMBO_SNIPING_FINISH_EventProcess();	// June 32af6c20
	void				LSI_LLC_F_DASH_COMBO_ZXXX_EventProcess();	// June 32af7960
	void				LSI_LLC_F_DASH_COMBO_XXX_EventProcess();	// June 32af7c10
	void				LSI_LLC_M_COMBO_ZZZX_EventProcess();	// June 32af7ec0
	void				LSI_LLC_M_DASH_COMBO_ZX_EventProcess();	// June 32af8230
	void				LSI_LLC_M_DASH_COMBO_ZXX_EventProcess();	// June 32af85a0
	void				LSI_LLC_M_DASH_COMBO_ZXXX_EventProcess();	// June 32af8910
	void				LSI_LLC_M_DASH_COMBO_XX_EventProcess();	// June 32af8bc0
	void				LSI_LLC_M_DASH_COMBO_XXX_EventProcess();	// June 32af8f30

	// char-luciel CH1g2: Chiliarch and Royal Guard combos part 2 ( June 32af91e0 - 32b3bc70 )
	void				LSI_LCG_F_COMBO_ZZZX_EventProcess();	// June 32af91e0
	void				LSI_LCG_F_COMBO_ZZZXX_END_EventProcess();	// June 32af9400
	void				LSI_LCG_F_COMBO_ZZZXZ_LOOP_EventProcess();	// June 32af9690
	void				LSI_LCG_F_COMBO_ZZZXZ_END_EventProcess();	// June 32af98c0
	void				LSI_LCG_F_DASH_COMBO_Z_EventProcess();	// June 32af9b90
	void				LSI_LCG_F_DASH_COMBO_ZX_EventProcess();	// June 32af9e80
	void				LSI_LCG_F_DASH_COMBO_XX_EventProcess();	// June 32afa2d0
	void				LSI_LCG_F_DASH_JUMP_COMBO_ZXX_EventProcess();	// June 32afa720
	void				LSI_LCG_M_DASH_JUMP_COMBO_ZXX_EventProcess();	// June 32afa750
	void				LSI_LCG_F_DASH_JUMP_COMBO_XZZ_READY_EventProcess();	// June 32afa780
	void				LSI_LCG_F_DASH_JUMP_COMBO_XZZ_LOOP_EventProcess();	// June 32afa7f0
	void				LSI_LCG_F_DASH_JUMP_COMBO_XZZ_END_EventProcess();	// June 32afaac0
	void				LSI_LLC_F_DASH_JUMP_COMBO_Z_DASH_EventProcess();	// June 32b105d0
	void				LSI_LLC_F_DASH_JUMP_COMBO_Z_DASH_UP_EventProcess();	// June 32b10980
	void				LSI_LLC_F_DASH_JUMP_COMBO_Z_DASH_DOWN_EventProcess();	// June 32b10bd0
	void				LSI_LLC_F_DASH_JUMP_COMBO_X_DASH_EventProcess();	// June 32b10e20
	void				LSI_LLC_F_DASH_JUMP_COMBO_X_DASH_UP_EventProcess();	// June 32b111d0
	void				LSI_LLC_F_DASH_JUMP_COMBO_X_DASH_DOWN_EventProcess();	// June 32b11420
	void				LSI_LLC_M_DASH_JUMP_COMBO_Z_DASH_EventProcess();	// June 32b11d80
	void				LSI_LLC_M_DASH_JUMP_COMBO_Z_DASH_UP_EventProcess();	// June 32b12130
	void				LSI_LLC_M_DASH_JUMP_COMBO_Z_DASH_DOWN_EventProcess();	// June 32b12380
	void				LSI_LLC_M_DASH_JUMP_COMBO_X_DASH_EventProcess();	// June 32b125d0
	void				LSI_LLC_M_DASH_JUMP_COMBO_X_DASH_UP_EventProcess();	// June 32b12980
	void				LSI_LLC_M_DASH_JUMP_COMBO_X_DASH_DOWN_EventProcess();	// June 32b12bd0
	void				LSI_LCG_COMBO_SNIPING_X_FIRE_StateStart();	// June 32b1cba0
	void				LSI_LCG_F_COMBO_ZZZXX_END_StateStart();	// June 32b1cc60
	void				LSI_LLC_COMBO_ZZZX_FrameMove();	// June 32b22240
	void				LSI_LCG_COMBO_SNIPING_Z_FrameMove();	// June 32b26030
	void				LSI_LCG_COMBO_SNIPING_X_FIRE_FrameMove();	// June 32b266a0
	void				LSI_LCG_DASH_JUMP_COMBO_SNIPING_LOOP_FrameMove();	// June 32b26a40
	void				LSI_LLC_M_COMBO_ZZZX_FrameMove();	// June 32b27b60
	void				LSI_LCG_F_COMBO_ZZZXX_END_FrameMove();	// June 32b28020
	void				LSI_LCG_F_COMBO_ZZZXZ_LOOP_FrameMove();	// June 32b282c0
	void				LSI_LCG_F_DASH_JUMP_COMBO_XZZ_LOOP_FrameMove();	// June 32b287b0
	void				LSI_WIN_StateStart();	// June 32b3a290
	void				LSI_LOSE_StateStart();	// June 32b3a2b0
	void				LSI_LCG_COMBO_SNIPING_READY_EventProcess();	// June 32b3b530
	void				LSI_LCG_F_COMBO_ZZZX_WAIT_EventProcess();	// June 32b3bc70
	CX2EffectSet::Handle	PlayRatedProjectileEffectSet( const WCHAR* pwszEffectSetName_, const bool bAddMaxCountModifier_ );	// June 32b28020 and its siblings, inline in each
	void				SetEffectSetLocalRotateZ( const CX2EffectSet::Handle hEffectSet_, const float fRotateDegree_ );	// June +0xe4 of the effect set instance, inline in each
	void				SetAirWalkSpeedByInput();	// June 32b105d0 and its siblings, inline in each
	// char-luciel CH1h: Dread Lord and Noblesse combos ( June 32ae5940 - 32b29570 )
	void				LSI_LLD_COMBO_XX_DOWN_XX_LOOP_StateStartFuture();	// June 32ae5940
	void				LSI_LCN_DASH_COMBO_ZX_DOWN_XX_LOOP_StateStartFuture();	// June 32ae5df0
	void				LSI_LLD_M_COMBO_XX_DOWN_XX_LOOP_StateStartFuture();	// June 32ae60d0
	void				LSI_LCN_F_DASH_COMBO_ZX_DOWN_XX_LOOP_StateStartFuture();	// June 32ae60f0
	void				LSI_LCN_F_DASH_COMBO_XX_DOWN_XX_LOOP_StateStartFuture();	// June 32ae6110
	void				LSI_LLD_COMBO_ZZZXZ_EventProcess();	// June 32af1180
	void				LSI_LLD_COMBO_XX_DOWN_X_EventProcess();	// June 32af13b0
	void				LSI_LLD_COMBO_XX_DOWN_XX_READY_EventProcess();	// June 32af16a0
	void				LSI_LLD_COMBO_XX_DOWN_XX_LOOP_EventProcess();	// June 32af16e0
	void				LSI_LLD_COMBO_XX_DOWN_XX_LOOP_X_EventProcess();	// June 32af1950
	void				LSI_LCN_DASH_COMBO_ZX_DOWN_X_StateStartFuture();	// June 32af6c50
	void				LSI_LCN_DASH_COMBO_ZX_DOWN_X_EventProcess();	// June 32af6cc0
	void				LSI_LCN_DASH_COMBO_ZX_DOWN_XX_LOOP_EventProcess();	// June 32af6fc0
	void				LSI_LCN_DASH_COMBO_ZX_DOWN_XX_END_EventProcess();	// June 32af72d0
	void				LSI_LCN_SNIPING_COMBO_UP_X_EventProcess();	// June 32af7500
	void				LSI_LCN_SNIPING_COMBO_DOWN_X_EventProcess();	// June 32af7730
	void				LSI_LLD_M_COMBO_ZZZXZ_EventProcess();	// June 32afabc0
	void				LSI_LLD_M_COMBO_XX_DOWN_X_EventProcess();	// June 32afae50
	void				LSI_LLD_M_COMBO_XX_DOWN_XX_READY_EventProcess();	// June 32afb1c0
	void				LSI_LLD_M_COMBO_XX_DOWN_XX_LOOP_EventProcess();	// June 32afb450
	void				LSI_LLD_M_COMBO_XX_DOWN_XX_END_EventProcess();	// June 32afb6c0
	void				LSI_LCN_F_COMBO_ZZZX_UP_X_EventProcess();	// June 32afb950
	void				LSI_LCN_F_COMBO_ZZZX_DOWN_X_EventProcess();	// June 32afbbe0
	void				LSI_LCN_F_COMBO_XXXX_UP_X_EventProcess();	// June 32afbe70
	void				LSI_LCN_F_COMBO_XXXX_DOWN_X_EventProcess();	// June 32afc100
	void				LSI_LCN_F_DASH_COMBO_ZXX_UP_X_EventProcess();	// June 32afc390
	void				LSI_LCN_F_DASH_COMBO_ZXX_DOWN_X_EventProcess();	// June 32afc620
	void				LSI_LCN_F_DASH_COMBO_XXX_UP_X_EventProcess();	// June 32afc8b0
	void				LSI_LCN_F_DASH_COMBO_XXX_DOWN_X_EventProcess();	// June 32afcb40
	void				LSI_LCN_F_DASH_COMBO_ZX_DOWN_X_StateStartFuture();	// June 32afcdd0
	void				LSI_LCN_F_DASH_COMBO_ZX_DOWN_X_EventProcess();	// June 32afce40
	void				LSI_LCN_F_DASH_COMBO_ZX_DOWN_XX_LOOP_EventProcess();	// June 32afd140
	void				LSI_LCN_F_DASH_COMBO_ZX_DOWN_XX_END_EventProcess();	// June 32afd450
	void				LSI_LCN_F_DASH_COMBO_XX_DOWN_X_StateStartFuture();	// June 32afd6e0
	void				LSI_LCN_F_DASH_COMBO_XX_DOWN_X_EventProcess();	// June 32afd750
	void				LSI_LCN_F_DASH_COMBO_XX_DOWN_XX_LOOP_EventProcess();	// June 32afda50
	void				LSI_LCN_F_DASH_COMBO_XX_DOWN_XX_END_EventProcess();	// June 32afdd60
	void				LSI_LCN_DASH_COMBO_ZX_DOWN_XX_LOOP_FrameMove();	// June 32b15b20
	void				LSI_LCN_F_DASH_COMBO_ZX_DOWN_XX_LOOP_FrameMove();	// June 32b15d40
	void				LSI_LCN_F_DASH_COMBO_XX_DOWN_XX_LOOP_FrameMove();	// June 32b15fe0
	void				LSI_LCN_DASH_COMBO_ZX_DOWN_X_FrameMove();	// June 32b270b0
	void				LSI_LCN_SNIPING_COMBO_UP_X_FrameMove();	// June 32b272c0
	void				LSI_LCN_SNIPING_COMBO_DOWN_X_FrameMove();	// June 32b27710
	void				LSI_LCN_F_COMBO_ZZZX_UP_X_FrameMove();	// June 32b28ca0
	void				LSI_LCN_F_COMBO_ZZZX_DOWN_X_FrameMove();	// June 32b29000
	void				LSI_LCN_F_DASH_COMBO_ZX_DOWN_X_FrameMove();	// June 32b29360
	void				LSI_LCN_F_DASH_COMBO_XX_DOWN_X_FrameMove();	// June 32b29570
	// char-luciel CH1i: Demonio script states ( June 32ae9b40 - 32b39e70 )
	void				LSI_LDB_F_DASH_JUMP_COMBO_XZ_LANDING_StateStartFuture();	// June 32ae9b40
	void				LSI_BOUNDING_TARGET_LU_STRIKER_EventProcess();	// June 32b08230
	void				LSI_BOUNDING_TARGET_LU_COMMAND_X_EventProcess();	// June 32b085b0
	void				LSI_BOUNDING_TARGET_LU_COMMAND_X_CIEL_STRIKER_EventProcess();	// June 32b08940
	void				LSI_BOUNDING_TARGET_LU_COMMAND_XX_EventProcess();	// June 32b08d30
	void				LSI_BOUNDING_TARGET_LU_COMMAND_XXX_EventProcess();	// June 32b090d0
	void				LSI_BOUNDING_TARGET_LU_COMMAND_XXXX_EventProcess();	// June 32b09470
	void				LSI_BOUNDING_TARGET_LU_COMMAND_XXXXX_EventProcess();	// June 32b09810
	void				LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXX_EventProcess();	// June 32b09bb0
	void				LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_EventProcess();	// June 32b09fa0
	void				LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_Z_START_EventProcess();	// June 32b0a380
	void				LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_Z_LOOP_EventProcess();	// June 32b0a3e0
	void				LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_Z_LANDING_EventProcess();	// June 32b0a420
	void				LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_ZZ_EventProcess();	// June 32b0a780
	void				LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_ZZZ_EventProcess();	// June 32b0ab20
	void				LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_ZZZX_EventProcess();	// June 32b0aec0
	void				LSI_BOUNDING_TARGET_CIEL_COMMAND_XXXXXXX_ZZZX_EventProcess();	// June 32b0b190
	void				LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_X_EventProcess();	// June 32b0b460
	void				LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_XX_START_EventProcess();	// June 32b0b7c0
	void				LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_XX_LOOP_EventProcess();	// June 32b0b820
	void				LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_XX_LANDING_EventProcess();	// June 32b0b860
	void				LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_XXZ_EventProcess();	// June 32b0bbc0
	void				LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_XXZX_EventProcess();	// June 32b0bf60
	void				LSI_LDB_F_DASH_COMBO_Z_EventProcess();	// June 32b0c220
	void				LSI_LDB_F_DASH_COMBO_ZX_EventProcess();	// June 32b0c580
	void				LSI_LDB_F_DASH_COMBO_ZXX_EventProcess();	// June 32b0c920
	void				LSI_LDB_F_DASH_COMBO_ZXXX_EventProcess();	// June 32b0ccc0
	void				LSI_LDB_F_DASH_COMBO_ZXXXX_EventProcess();	// June 32b0d060
	void				LSI_LDB_F_DASH_COMBO_ZXXXXZ_EventProcess();	// June 32b0d400
	void				LSI_LDB_F_DASH_JUMP_COMBO_XZ_START_EventProcess();	// June 32b0d6d0
	void				LSI_LDB_F_DASH_JUMP_COMBO_XZ_LANDING_EventProcess();	// June 32b0d730
	void				LSI_LDB_F_DASH_JUMP_COMBO_XZ_LANDING_ATTACK_EventProcess();	// June 32b0d770
	void				LSI_LDB_F_DASH_JUMP_COMBO_XZX_EventProcess();	// June 32b0dac0
	void				LSI_LDB_M_DASH_JUMP_COMBO_XZ_LANDING_EventProcess();	// June 32b0dd90
	void				LSI_LDB_M_DASH_JUMP_COMBO_XZ_LANDING_ATTACK_EventProcess();	// June 32b0ddd0
	void				LSI_LDB_M_DASH_JUMP_COMBO_XZX_EventProcess();	// June 32b0e120
	void				LSI_BOUNDING_TARGET_LU_STRIKER_StateEnd();	// June 32b14c00
	void				LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_X_StateStartFuture();	// June 32b14c30
	void				ShowMinorParticleHyperModeShockOfBody();	// June 32b1f2b0
	void				LSI_BOUNDING_TARGET_LU_STRIKER_StateStartFuture();	// June 32b34100
	void				LSI_BOUNDING_TARGET_LU_COMMAND_X_StateStartFuture();	// June 32b341b0
	void				LSI_BOUNDING_TARGET_LU_COMMAND_XXXXXXX_Z_START_StateStartFuture();	// June 32b343b0
	void				LSI_BOUNDING_TARGET_LU_COMMAND_X_CIEL_STRIKER_StateStartFuture();	// June 32b35fd0
	void				LSI_RIDING_HYPER_MODE_FrameMove();	// June 32b39e70
	void				CommonHyperModeFrameMove( const float fChangeTime_, const float fShockTime_, const bool bSound_ );	// June 32b37d80 ( shared by the Lu / Ciel hyper mode states )

#ifdef SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL
	// char-luciel CH2a: Duo ( 200 ) skills ( June 32ae61b0 - 32b29fb0 )
	void				LSI_SA_LD_TRIPLE_SHOT_Init();	// June 32ae61b0
	void				LSI_SA_LD_TRIPLE_SHOT_FrameMove();	// June 32ae6240
	void				LSI_SA_LD_TRIPLE_SHOT_EventProcess();	// June 32afdff0
	void				LSI_SA_LD_STOMP_START_Init();	// June 32ae6250
	void				LSI_SA_LD_STOMP_START_FrameMove();	// June 32ae62e0
	void				LSI_SA_LD_STOMP_START_EventProcess();	// June 32afe1f0
	void				LSI_SA_LD_STOMP_LANDING_FrameMove();	// June 32ae62f0
	void				LSI_SA_LD_STOMP_LANDING_EventProcess();	// June 32afe250
	void				LSI_SA_LD_FLICK_SHOT_Init();	// June 32ae6300
	void				LSI_SA_LD_FLICK_SHOT_FrameMove();	// June 32b29780
	void				LSI_SA_LD_FLICK_SHOT_EventProcess();	// June 32afe450
	void				LSI_SA_LD_EVIL_CLAW_Init();	// June 32ae63c0
	void				LSI_SA_LD_EVIL_CLAW_FrameMove();	// June 32b29af0
	void				LSI_SA_LD_EVIL_CLAW_EventProcess();	// June 32afe650
	void				LSI_SA_LD_SOUL_INFEST_Init();	// June 32ae6480
	void				LSI_SA_LD_SOUL_INFEST_FrameMove();	// June 32b29fb0
	void				LSI_SA_LD_SOUL_INFEST_EventProcess();	// June 32afe850
	void				LSI_SA_LD_NETHER_SHREDDER_Init();	// June 32ae65a0
	void				LSI_SA_LD_NETHER_SHREDDER_FrameMove();	// June 32ae66a0
	void				LSI_SA_LD_NETHER_SHREDDER_EventProcess();	// June 32afea50
	void				LSI_A_LD_F_HEAVY_HIT_Init();	// June 32ae66c0
	void				LSI_A_LD_F_HEAVY_HIT_StateStart();	// June 32b1cf90
	void				LSI_A_LD_F_HEAVY_HIT_FrameMove();	// June 32ae6780
	void				LSI_A_LD_F_HEAVY_HIT_EventProcess();	// June 32afee50
	void				LSI_A_LD_M_HEAVY_HIT_StateStart();	// June 32b1cd20
	void				LSI_A_LD_M_HEAVY_HIT_FrameMove();	// June 32ae66b0
	void				LSI_A_LD_M_HEAVY_HIT_EventProcess();	// June 32afec50
	void				LSI_A_LD_F_HAUNTING_BEAT_Init();	// June 32ae67a0
	void				LSI_A_LD_F_HAUNTING_BEAT_FrameMove();	// June 32ae6870
	void				LSI_A_LD_F_HAUNTING_BEAT_EventProcess();	// June 32aff250
	void				LSI_A_LD_M_HAUNTING_BEAT_FrameMove();	// June 32ae6790
	void				LSI_A_LD_M_HAUNTING_BEAT_EventProcess();	// June 32aff050
	// char-luciel CH2b: Chiliarch ( 37 ) skills ( June 32ae6900 - 32b3d010 )
	bool				IsSkillOrUpgrade( const int iSkillID_, const int iTarget_ ) const;	// June 31edf560
	void				LSI_SA_LLC_DEAD_HANDS_Init();	// June 32ae6900
	void				LSI_SA_LLC_DEAD_HANDS_StateStartFuture();	// June 32ae6a30
	void				LSI_SA_LLC_DEAD_HANDS_FrameMoveFuture();	// June 32ae6a60
	void				LSI_SA_LLC_DEAD_HANDS_FrameMove();	// June 32b13180
	void				LSI_SA_LLC_DEAD_HANDS_EventProcess();	// June 32aff850
	void				LSI_SA_LLC_DARK_CRESCENT_Init();	// June 32ae6a70
	void				LSI_SA_LLC_DARK_CRESCENT_StateStartFuture();	// June 32b2ac20
	void				LSI_SA_LLC_DARK_CRESCENT_StateStart();	// June 32affa70
	void				LSI_SA_LLC_DARK_CRESCENT_FrameMove();	// June 32b3d010
	void				LSI_SA_LLC_DARK_CRESCENT_EventProcess();	// June 32affa90
	void				LSI_SA_LLC_DARK_CRESCENT_StateEndFuture();	// June 32b1d200
	void				LSI_SA_LLC_RUINOUS_IMPACT_Init();	// June 32ae6ad0
	void				LSI_SA_LLC_RUINOUS_IMPACT_StateStart();	// June 32affca0
	void				LSI_SA_LLC_RUINOUS_IMPACT_FrameMoveFuture();	// June 32ae6b40
	void				LSI_SA_LLC_RUINOUS_IMPACT_FrameMove();	// June 32ae6b30
	void				LSI_SA_LLC_RUINOUS_IMPACT_EventProcess();	// June 32b16ca0
	void				LSI_SA_LLC_POWER_OF_ORIAX_Init();	// June 32ae6b80
	void				LSI_SA_LLC_POWER_OF_ORIAX_FrameMove();	// June 32ae6be0
	void				LSI_SA_LLC_POWER_OF_ORIAX_EventProcess();	// June 32affcf0
	void				LSI_SA_LLC_APOLLYON_Init();	// June 32ae6bf0
	void				LSI_SA_LLC_APOLLYON_StateStart();	// June 32ae6ce0
	void				LSI_SA_LLC_APOLLYON_FrameMove();	// June 32b2b050
	void				LSI_SA_LLC_APOLLYON_EventProcess();	// June 32affef0
	void				LSI_A_LLC_F_BLASTING_FEAR_Init();	// June 32ae7920
	void				LSI_A_LLC_F_BLASTING_FEAR_EventProcess();	// June 32b01050, also the male body's
	void				LSI_A_LLC_F_BACKSTAB_Init();	// June 32ae7980
	void				LSI_A_LLC_F_BACKSTAB_EventProcess();	// June 32b01250, also the male body's
	// char-luciel CH2c: Royal Guard ( 38 ) skills part 1 ( June 32ae6e10 - 32b00e10 ); the setter cases other than Soul Extortion wait for CH2c2's callbacks
	void				LSI_SA_LCG_SOUL_EXTORTION_Init();	// June 32ae6e10
	void				LSI_SA_LCG_SOUL_EXTORTION_FrameMove();	// June 32ae6eb0
	void				LSI_SA_LCG_SOUL_EXTORTION_StateStartFuture();	// June 32ae6ec0
	void				LSI_SA_LCG_SOUL_EXTORTION_StateStart();	// June 32ae6f30
	void				LSI_SA_LCG_SOUL_EXTORTION_EventProcess();	// June 32b002f0
	void				LSI_SA_LCG_DEMONIC_BREATH_START_Init();	// June 32ae6fa0
	void				LSI_SA_LCG_DEMONIC_BREATH_CHARGING_StateStart();	// June 32b004f0
	void				LSI_SA_LCG_DEMONIC_BREATH_FIRE_StateStartFuture();	// June 32ae70f0
	void				LSI_SA_LCG_DEMONIC_BREATH_FIRE_FrameMoveFuture();	// June 32ae7110
	void				LSI_SA_LCG_DEMONIC_BREATH_FIRE_EventProcess();	// June 32b00510
	void				LSI_SA_LCG_FANTASIE_IMPROMPTU_CANTABILE_READY_Init();	// June 32ae7150
	void				LSI_SA_LCG_FANTASIE_IMPROMPTU_CANTABILE_READY_StateStart();	// June 32ae7280
	void				LSI_SA_LCG_FANTASIE_IMPROMPTU_CANTABILE_READY_EventProcess();	// June 32b005f0
	void				LSI_SA_LCG_FANTASIE_IMPROMPTU_CANTABILE_WAIT_EventProcess();	// June 32ae72c0
	void				LSI_SA_LCG_FANTASIE_IMPROMPTU_CANTABILE_FRONT_MOVE_EventProcess();	// June 32ae7350
	void				LSI_SA_LCG_FANTASIE_IMPROMPTU_CANTABILE_BACK_MOVE_EventProcess();	// June 32ae73e0
	void				LSI_SA_LCG_FANTASIE_IMPROMPTU_ACCELERANDO_READY_StateStart();	// June 32ae7470
	void				LSI_SA_LCG_FANTASIE_IMPROMPTU_ACCELERANDO_READY_EventProcess();	// June 32b00800
	void				LSI_SA_LCG_FANTASIE_IMPROMPTU_ACCELERANDO_WAIT_EventProcess();	// June 32ae7490
	void				LSI_SA_LCG_FANTASIE_IMPROMPTU_ACCELERANDO_FRONT_MOVE_EventProcess();	// June 32ae7520
	void				LSI_SA_LCG_FANTASIE_IMPROMPTU_ACCELERANDO_BACK_MOVE_EventProcess();	// June 32ae75b0
	void				LSI_SA_LCG_DEATH_CRISIS_OF_MARBAS_READY_Init();	// June 32ae7640
	void				LSI_SA_LCG_DEATH_CRISIS_OF_MARBAS_READY_StateStart();	// June 32b00840
	void				LSI_SA_LCG_DEATH_CRISIS_OF_MARBAS_READY_EventProcess();	// June 32b00920
	void				LSI_SA_LCG_DEATH_CRISIS_OF_MARBAS_WAIT_StateEndFuture();	// June 32ae7700
	void				LSI_SA_LCG_DEATH_CRISIS_OF_MARBAS_SMALL_FIRE_StateStartFuture();	// June 32b00960
	void				LSI_SA_LCG_DEATH_CRISIS_OF_MARBAS_BIG_FIRE_StateStartFuture();	// June 32b009a0
	void				LSI_SA_LCG_DEATH_CRISIS_OF_MARBAS_BIG_FIRE_EventProcess();	// June 32b009e0
	void				LSI_SA_LCG_DEATH_CRISIS_OF_MARBAS_END_StateStart();	// June 32ae7720
	void				LSI_SA_LCG_DEATH_CRISIS_OF_MARBAS_END_EventProcess();	// June 32b00c10
	void				LSI_SA_LCG_ESCHATON_Init();	// June 32ae7730
	void				LSI_SA_LCG_ESCHATON_EventProcess();	// June 32b00e10
	void				LSI_A_LCG_F_SOUL_SHOCK_Init();	// June 32ae79f0
	void				LSI_A_LCG_F_SOUL_STRIKE_READY_Init();	// June 32ae7a80
	void				LSI_A_LCN_F_SHADOW_Init();	// June 32ae84a0
	// char-luciel CH2c2: Royal Guard ( 38 ) skills part 2, the event processes / frame moves / state ends the CH2c states still lacked
	void				LSI_A_LCG_F_SOUL_SHOCK_EventProcess();	// June 32b01450, also the male body's
	void				LSI_A_LCG_F_SOUL_SHOCK_FrameMove();	// June 32b2db60
	void				LSI_A_LCG_M_SOUL_SHOCK_FrameMove();	// June 32b2e080
	void				LSI_A_LCG_SOUL_SHOCK_PlayEffects( const WCHAR* pwszEffectSetName_ );	// June 32b2db60 / 32b2e080: the two bodies differ only in the effect set name
	void				LSI_A_LCG_F_SOUL_STRIKE_READY_EventProcess();	// June 32b01650
	void				LSI_A_LCG_M_SOUL_STRIKE_READY_EventProcess();	// June 32b01890
	void				LSI_A_LCG_F_SOUL_STRIKE_END_EventProcess();	// June 32b01690, also the male body's
	void				LSI_SA_LCG_DEMONIC_BREATH_START_StateStartFuture();	// June 32b1d2a0
	void				LSI_SA_LCG_DEMONIC_BREATH_START_EventProcess();	// June 32b1d350
	void				LSI_SA_LCG_DEMONIC_BREATH_CHARGING_EventProcess();	// June 32b1d490
	void				LSI_SA_LCG_DEMONIC_BREATH_FIRE_FrameMove();	// June 32b2ba70
	bool				LSI_SA_LCG_DEMONIC_BREATH_CountKeyPress();	// June 32b1d350 / 32b1d490: the same inlined block, one press spends MP and counts
	void				LSI_SA_LCG_ESCHATON_FrameMove();	// June 32b2cfc0
	void				LSI_SA_LCG_FANTASIE_IMPROMPTU_CANTABILE_WAIT_FrameMoveFuture();	// June 32b13260, also the front / back move and the 2nd movement's three states
	void				LSI_SA_LCG_FANTASIE_IMPROMPTU_CANTABILE_WAIT_FrameMove();	// June 32b2bf20, also the front / back move
	void				LSI_SA_LCG_FANTASIE_IMPROMPTU_ACCELERANDO_WAIT_FrameMove();	// June 32b2c270, also the front / back move
	void				LSI_SA_LCG_FANTASIE_IMPROMPTU_ACCELERANDO_END_EventProcess();	// June 32b1d5f0
	void				SetLeftHandRotateBone();	// June 32b15680: Death Crisis of Marbas turns the left hand bone with the unit's aim value
	void				LSI_SA_LCG_DEATH_CRISIS_OF_MARBAS_WAIT_FrameMove();	// June 32b16200, also the end state's
	void				LSI_SA_LCG_DEATH_CRISIS_OF_MARBAS_READY_StateStartFuture();	// June 32b1d8d0
	void				LSI_SA_LCG_DEATH_CRISIS_OF_MARBAS_WAIT_EventProcess();	// June 32b1d970
	void				LSI_SA_LCG_DEATH_CRISIS_OF_MARBAS_BIG_CHARGE_EventProcess();	// June 32b1dc50
	void				LSI_SA_LCG_DEATH_CRISIS_OF_MARBAS_SMALL_FIRE_EventProcess();	// June 32b1de10
	void				LSI_SA_LCG_DEATH_CRISIS_OF_MARBAS_BIG_CHARGE_FrameMove();	// June 32b2c5e0
	void				LSI_SA_LCG_DEATH_CRISIS_OF_MARBAS_SMALL_FIRE_FrameMove();	// June 32b2c7d0
	void				LSI_SA_LCG_DEATH_CRISIS_OF_MARBAS_BIG_FIRE_FrameMove();	// June 32b2cd70
	void				LSI_SA_LCG_DEATH_CRISIS_OF_MARBAS_UpdateAimValue();	// June 32b1d970 / 32b1dc50 / 32b1de10: the same inlined tail of the three event processes
	bool				LSI_SA_LCG_DEATH_CRISIS_OF_MARBAS_FlushMpByCharge( const double dChargeRate_ );	// June 32b1d970 / 32b1de10: the MP of a fire or the charge, inlined at every site
	void				LSI_A_LCG_F_SOUL_STRIKE_READY_StateEndFuture();	// June 32b1e490, also the male ready state's
	void				LSI_A_LCG_SOUL_STRIKE_LOOP_ProcessAnimationEnd( const int iLoopStateID_, const int iEndStateID_ );	// June 32b1e540 / 32b1e630: the same inlined block
	void				LSI_A_LCG_F_SOUL_STRIKE_LOOP_EventProcess();	// June 32b1e540
	void				LSI_A_LCG_M_SOUL_STRIKE_LOOP_EventProcess();	// June 32b1e630
	void				LSI_A_LCG_F_SOUL_STRIKE_READY_StateEnd();	// June 32b3dea0, also the male ready state's
	float				LSI_A_LCG_SOUL_STRIKE_LOOP_ApplyStackRates();	// June 32b2e5a0 / 32b2ec50: the same inlined block, returns the size rate
	void				LSI_A_LCG_F_SOUL_STRIKE_LOOP_FrameMove();	// June 32b2e5a0
	void				LSI_A_LCG_M_SOUL_STRIKE_LOOP_FrameMove();	// June 32b2ec50
	void				ApplyShadowBuff();	// June 32b20670: Shadow's buff of the active body and the charge value from the max HP
	void				LSI_A_LCN_SHADOW_FrameMoveBody();	// June 32b30bc0 / 32b30d40: the two bodies' frame moves are the same listing
	void				LSI_A_LCN_F_SHADOW_FrameMove();	// June 32b30bc0
	void				LSI_A_LCN_M_SHADOW_FrameMove();	// June 32b30d40
	void				LSI_SA_LCG_SOUL_EXTORTION_StateEnd();	// June 32b3dd80
	void				AddSoulCollectorStack();	// June 32b3db80: one more Soul Collector stack, the Tribute / Soul Strike side effects ride with it ( four callers, two in CH2c2 )
	// char-luciel CH2d: Diablura ( 39 ) skills part 1; the setter cases of Nightmare, Altar of Evil, Abyss Strike and Phantom Rush wait for CH2d2
	void				LSI_SA_LDB_LAND_CRASHER_Init();	// June 32ae87b0
	void				LSI_SA_LDB_LAND_CRASHER_EventProcess();	// June 32b02840
	void				LSI_SA_LDB_LAND_CRASHER_LANDING_EventProcess();	// June 32b02890
	void				LSI_SA_LDB_BLOODLUST_Init();	// June 32ae88a0
	void				LSI_SA_LDB_BLOODLUST_EventProcess();	// June 32b02b00
	void				LSI_SA_LDB_NIGHTMARE_Init();	// June 32ae88f0
	void				LSI_SA_LDB_NIGHTMARE_StateStart();	// June 32ae89c0
	void				LSI_SA_LDB_ALTAR_OF_EVIL_Init();	// June 32ae89e0
	void				LSI_SA_LDB_ALTAR_OF_EVIL_EventProcess();	// June 32b02d80
	void				LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_START_EventProcess();	// June 32b02dd0
	void				LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_JUMP_UP_EventProcess();	// June 32ae8b60
	void				LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_JUMP_DOWN_EventProcess();	// June 32b02e10
	void				LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_JUMP_X_EventProcess();	// June 32b02e60
	void				LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_COMBO_Z_EventProcess();	// June 32b02f10
	void				LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_COMBO_ZZ_EventProcess();	// June 32b03250
	void				LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_COMBO_ZZZ_EventProcess();	// June 32b03590
	void				LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_COMBO_ZZZZ_EventProcess();	// June 32b038d0
	void				LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_COMBO_X_EventProcess();	// June 32b03bb0
	void				LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_COMBO_XX_EventProcess();	// June 32b03ef0
	void				LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_COMBO_XXX_READY_EventProcess();	// June 32b04230
	void				LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_COMBO_XXX_READY_SECOND_EventProcess();	// June 32b045b0
	void				LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_COMBO_XXX_END_EventProcess();	// June 32b042d0
	void				LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_DEATH_TOUCH_Init();	// June 32ae8bb0
	void				LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_MORTAL_FEAR_Init();	// June 32ae8c50
	void				LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_MORTAL_FEAR_StateStart();	// June 32ae8cf0
	void				LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_DARK_PLASMA_Init();	// June 32ae8d10
	void				LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_DARK_PLASMA_StateStart();	// June 32ae8e00
	void				LSI_A_LDB_F_ABYSS_STRIKE_Init();	// June 32ae8f50, also the male state's
	void				LSI_A_LDB_F_PHANTOM_RUSH_Init();	// June 32ae8ff0
	void				LSI_A_LDB_M_PHANTOM_RUSH_Init();	// June 32ae90b0
	CX2SkillTree::SKILL_ID	GetTransformedSkillID( const int iSlotIndex_ ) const;	// June 32ae9c90: the linked skill of slot 0-2
	// char-luciel CH2d2: Diablura ( 39 ) skills part 2; the setter cases of Nightmare, Altar of Evil, Abyss Strike and Phantom Rush
	bool				CollectNightmareTargetUnit();	// June 32b17090: the enemies in front within range, kept in m_vecNightmareTargetUnit
	void				SetTransformBuffFactorToSelf( const UINT uiBuffFactorID_, const UINT uiSkillLevel_ );	// June 32b3e720: the form's buff factor at the skill's level
	void				CreateAltarOfEvilProjectile();	// June 32b358b0
	void				LSI_SA_LDB_NIGHTMARE_StateStartFuture();	// June 32b16f50
	void				LSI_SA_LDB_NIGHTMARE_EventProcess();	// June 32b178b0
	void				LSI_SA_LDB_NIGHTMARE_TARGET_StateStartFuture();	// June 32b30ec0
	void				LSI_SA_LDB_NIGHTMARE_TARGET_FrameMove();	// June 32b13360
	void				LSI_SA_LDB_NIGHTMARE_TARGET_EventProcess();	// June 32b17c20
	void				LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_START_StateStartFuture();	// June 32b43910
	void				LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_START_StateStart();	// June 32b43ae0
	void				LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_WAIT_EventProcess();	// June 32b31260
	void				LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_WALK_EventProcess();	// June 32b31480
	void				LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_JUMP_READY_EventProcess();	// June 32b133d0
	void				LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_JUMP_LANDING_EventProcess();	// June 32b316a0
	void				LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_DASH_EventProcess();	// June 32b36290
	void				LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_DASH_END_EventProcess();	// June 32b364e0
	void				LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_DASH_JUMP_EventProcess();	// June 32b14180
	void				LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_DASH_JUMP_LANDING_EventProcess();	// June 32b36750
	void				LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_JUMP_Z_EventProcess();	// June 32b13500
	void				LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_COMBO_XXX_READY_StateStart();	// June 32b380a0
	void				LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_COMBO_XXX_READY_SECOND_StateStart();	// June 32b380f0
	void				LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_DEATH_TOUCH_StateStart();	// June 32b38110
	void				LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_DEATH_TOUCH_FrameMove();	// June 32b318e0
	void				LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_DEATH_TOUCH_EventProcess();	// June 32b04640, also Mortal Fear's and Dark Plasma's
	void				LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_END_EventProcess();	// June 32b048c0
	void				LSI_SA_LDB_ALTAR_OF_EVIL_TRANSFORM_END_StateEnd();	// June 32b3a2d0
	void				LSI_SA_LDB_ALTAR_OF_EVIL_END_EventProcess();	// June 32b04910
	void				LSI_A_LDB_F_ABYSS_STRIKE_EventProcess();	// June 32b04d30, also the male state's
	void				LSI_A_LDB_F_PHANTOM_RUSH_FrameMove();	// June 32b38140
	void				LSI_A_LDB_M_PHANTOM_RUSH_FrameMove();	// June 32b38340 ( the class map's CameraMove: June registers it in the FrameMove slot )
	void				LSI_A_LDB_F_PHANTOM_RUSH_TARGET_StateStartFuture();	// June 32b38540, also the male target state's
	// char-luciel CH2d3: the transformed form's start, from the form buff's hook
	virtual void		SetSpecificValueByBuffTempletID( const BUFF_TEMPLET_ID eBuffTempletId_ );	// June 32b37d40 ( override 0x24c )
	void				StartTransformedForm( const int iForm_ );	// June 32b34fe0
#endif SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL
	// char-luciel CH1l: helpers the CH1l overrides call, bodies in CH1l3 ( names by what each does; CH1l3 keeps them )
	CKTDGParticleSystem::CParticleEventSequence*	SetLucielMinorParticleByEnum( LUCIEL_MINOR_PARTICLE_INSTANCE_ID eVal_, wstring wstrParticleName_, int iDrawCount_ = -1 );	// June 32b1eca0: body in CH1l3
	void				CreateChaserOfDarknessDamageEffect( const CX2DamageManager::DamageData* pDamageData_ );	// June 32b1f800: body in CH1l3
	void				ProcessPhantomRushHit( const CX2DamageManager::DamageData* pDamageData_ );	// June 32b347f0: body in CH1l3
	void				PlayPunisherEffectSet( const CX2DamageManager::DamageData* pDamageData_ );	// June 32b34c70: body in CH1l3
	void				RegisterDesperadoTarget( const CX2DamageManager::DamageData* pDamageData_ );	// June 32b18060: body in CH1l3
	void				AccumulateShadowFuryDamage( const CX2DamageManager::DamageData* pDamageData_ );	// June 32b0e4e0: body in CH1l3
	void				PlayBluffEffectSet( const CX2DamageManager::DamageData* pDamageData_ );	// June 32b37090: body in CH1l3
	void				CheckBoundingStrikeHit( const CX2DamageManager::DamageData* pDamageData_ );	// June 32b153d0: body in CH1l3
	void				AddCombinationPointByDamageEffectHit( const CX2DamageManager::DamageData* pDamageData_ );	// June 32b16880: body in CH1l3
	void				GetFormEquipItemList( const CX2Unit::EQIP_POSITION eEquipPosition_, std::vector<SFormEquipItem>& vecFormEquipItem_ );	// June 32b176a0: body in CH1l3

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
	CX2DamageEffect::CEffectHandle		m_hSoulShockDamageEffect;			// +0x963c: a damage effect handle ( June 32b397a0 moves it with the unit )
	float								m_fCommunionRate;					// +0x9640
	float								m_afFormMpRegenerationRate[2];		// +0x9644: June's ctor leaves them unset
	float								m_fComboXXDownXXLoopTime;			// +0x964c
	CX2DamageEffect::CEffectHandle		m_hDashComboDownXDamageEffect;		// +0x9650: a damage effect handle ( 32b270b0 )
	D3DXVECTOR3							m_vDashComboDownXPosition;			// +0x9654
	USHORT								m_usDancingBladeLoopCount;			// +0x9660
	float								m_fSummonedDarknessElapsedTime;		// +0x9664
	int									m_iLordOfFearSkillLevel;			// +0x9668
	float								m_fReduceDebuffDurationRate;		// +0x966c
	float								m_fLordOfFearDamageRate;			// +0x9670
	float								m_fFearDamageEffectCoolTime;		// +0x9674
	UCHAR								m_ucAuraOfFearStackCount;			// +0x9678: char-luciel CH1l3: a count ( June 32b15780 caps it at 10 ), CH1a named it a bool
	float								m_fAuraOfFearTime;					// +0x967c
	float								m_fAuraOfFearDuration;				// +0x9680
	float								m_fEnhancedSpecterRate;				// +0x9684
	float								m_fEnhancedSpecterRange;			// +0x9688
	float								m_fShadowShieldAmount;				// char-luciel CH2c2 review: +0x968c, Shadow's absorb pool ( max HP x rate ), drained by June 32aebaf0
	int									m_iShadowBoltNpcUID;				// +0x9690: char-luciel CH1l3: the Shadow Bolt spear's NPC UID ( June 32b20780 looks it up by UID ), CH1a named it an index
	float								m_fShadowBoltTime;					// +0x9694
	float								m_fShadowBoltDuration;				// +0x9698
	UCHAR								m_ucTwilightJudgementStackCount;	// +0x969c: char-luciel CH1l3: the Twilight Judgement buff's stack count ( June 32aebb60 ), CH1a named it a bool
	float								m_fTwilightJudgementTime;			// +0x96a0
	float								m_fSoulStrikeCoolTime;				// +0x96a4
	float								m_fCombinationPointOnHitCoolTime;	// +0x96a8
	CX2EffectSet::Handle				m_hDarkCrescentEffectSet;			// +0x96ac
	int									m_iEvilClawMemoVariant;				// +0x96b0: char-luciel CH2a: the Evil Claw skill-memo set index ( 0 base, 1 / 2 the memo sets ), CH1a named it a hit count
	float								m_fSwitchingCoolTime;				// +0x96b4
	int									m_iSkillUseMpForm;					// +0x96b8: the form that pays a skill's MP ( SpecialAttackUse stores 0 for Lu, 1 for Ciel )
	int									m_iPendingCombinationPoint;			// +0x96bc: char-luciel CH1p: a state's CP_AMOUNT, paid at the next SpecialAttackCooldown ( CH1a named it the last used skill id )
	float								m_fSoulStrikeMoveSpeed;				// +0x96c0
	float								m_fSoulStrikeSpeedRate;				// +0x96c4
	std::vector<SGameUnitKey>			m_vecDarkCrescentTargetUnit;		// +0x96c8
	int									m_iBoundingStrikeCombinationPointCost;	// +0x96dc: char-luciel CH1p: 0 or 1, rolled by SetBoundingTarget; June's ctor leaves it unset
	bool								m_bBoundingStrikeHit;				// +0x96e0: June's ctor leaves it unset
	bool								m_bBoundingTargetFound;				// +0x96e8
	int									m_iBoundingTargetLastHitAttackSerial;	// +0x96ec: char-luciel CH1p: SetBoundingTarget tests it, only CheckBoundingTarget writes it ( -1 )
	SGameUnitKey						m_BoundingTarget;					// +0x96f0
	float								m_fBoundingKeyShowTime;				// +0x9700
	CKTDGUIDialogType					m_pDlgBoundingKey;					// +0x9704
	bool								m_bPhantomRushReady;				// +0x9710
	float								m_fPhantomRushReadyTime;			// +0x9714
	CX2EffectSet::Handle				m_hPhantomRushEffectSet;			// +0x9718
	SGameUnitKey						m_PhantomRushTarget;				// +0x9720
	KProtectedType<float>				m_fFreeBoundingStrikeRate;			// +0x9730: char-luciel CH1p: GetRandomFloat( 0 ) <= it makes the bounding strike free ( CH4a's passive sets it )
	KProtectedType<float>				m_fBoundingDamageRateEx;			// +0x973c
	SAltarOfEvilData					m_AltarOfEvilData;					// +0x9748
	bool								m_bNightmareTargetHit;				// +0x9778: June's ctor leaves it unset
	std::vector<CX2GameUnit*>			m_vecNightmareTargetUnit;			// +0x977c
	std::vector<SGameUnitKey>			m_vecNightmareHitUnit;				// +0x978c
	float								m_fDeathTouchTime;					// +0x97a0
	float								m_fDeathTouchRange;					// +0x97a4
	SGameUnitKey						m_DeathTouchTarget;					// +0x97a8
	CX2DamageEffect::CEffectHandle		m_hDeathTouchBeamEffect;			// +0x97b8: char-luciel CH1l3: damage effect handles ( June 32b349f0 / 32b34a90 ); CH1a named the pair one UidType
	CX2DamageEffect::CEffectHandle		m_hDeathTouchTargetEffect;			// +0x97bc
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
#ifdef SERV_IRUHADEV_JUNE_CHAR_LOCKON_SYSTEM
	sLockOnSystemPtr					m_ptrShrewdCutterLockOn;			// char-luciel CH3c20: +0x9830, CX2GUUser's type
#else SERV_IRUHADEV_JUNE_CHAR_LOCKON_SYSTEM
	CX2GUAra::sLockOnSystemPtr			m_ptrShrewdCutterLockOn;			// +0x9830: Ara's type until CH3c20 moves it out of CX2GUAra
#endif SERV_IRUHADEV_JUNE_CHAR_LOCKON_SYSTEM
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
	bool								m_bSkillCameraOverride;				// +0x8d90 ( CX2GUUser's camera group ): stand-in, the reader is Stage 9's camera pair; CommonStateStart clears it
	float								m_fSkillCameraOverrideValue;		// +0x8da4 ( CX2GUUser's camera group ): stand-in, as above
#ifdef SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL
	float								m_fSkillCameraOverrideSecondValue;	// +0x8d94 ( CX2GUUser's camera group, ctor 0 ): char-luciel CH2c: stand-in as above, Death Crisis of Marbas sets it to 220.0 ( June 32b00840 )
#endif SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL
#ifdef SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL
	CX2SkillTree::SKILL_ID				m_eStateStartSkillID;				// +0x9444 ( CX2GUUser ): char-luciel CH2b: the skill of the state that last ran CommonStateStart ( June 32057b80 @32060cd9 )
#endif SERV_IRUHADEV_JUNE_CHAR_LUCIEL_SKILL
};

#endif SERV_IRUHADEV_JUNE_CHAR_LUCIEL_CORE
//}}
