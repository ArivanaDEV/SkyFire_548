/*
 * Copyright (C) 2011-2022 Project SkyFire <https://www.projectskyfire.org/>
 * Copyright (C) 2008-2022 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2022 MaNGOS <https://www.getmangos.eu/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _VMAPDEFINITIONS_H
#define _VMAPDEFINITIONS_H
#include <cstring>

#define LIQUID_TILE_SIZE (533.333f / 128.f)

namespace VMAP
{
    const char VMAP_MAGIC[] = "VMAP_5.2";
    const char RAW_VMAP_MAGIC[] = "VMAP052";                // used in extracted vmap files with raw data
    const char GAMEOBJECT_MODELS[] = "GameObjectModels.dtree";

    // defined in TileAssembler.cpp currently...
    bool readChunk(FILE* rf, char *dest, const char *compare, uint32 len);
}

// Set of helper macros for extractors (VMAP and MMAP)
#ifndef NO_CORE_FUNCS
#define VMAP_ERROR_LOG(FILTER, ...) SF_LOG_ERROR(FILTER, __VA_ARGS__)
#define VMAP_DEBUG_LOG(FILTER, ...) SF_LOG_DEBUG(FILTER, __VA_ARGS__)
#define VMAP_INFO_LOG(FILTER, ...) SF_LOG_INFO(FILTER, __VA_ARGS__)
#else
#define VMAP_ERROR_LOG(FILTER, ...) (void)sizeof(FILTER)
#define VMAP_DEBUG_LOG(FILTER, ...) (void)sizeof(FILTER)
#define VMAP_INFO_LOG(FILTER, ...)  (void)sizeof(FILTER)
#endif

#endif
