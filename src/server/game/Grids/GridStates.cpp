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

#include "GridStates.h"
#include "GridNotifiers.h"
#include "Log.h"

#ifdef SKYFIRE_DEBUG
bool GridState::checkMagic()
{
    if (i_Magic != MAGIC_TESTVAL)
    {
        SF_LOG_ERROR("misc", "!!! GridState: Magic value gone !!!");
        return false;
    }
    return true;
}
#endif

void InvalidState::Update(Map&, NGridType&, GridInfo&, uint32) const
{ }

void ActiveState::Update(Map& map, NGridType& grid, GridInfo&  info, uint32 diff) const
{
    // Only check grid activity every (grid_expiry/10) ms, because it's really useless to do it every cycle
    info.UpdateTimeTracker(diff);
    if (info.getTimeTracker().Passed())
    {
        if (!grid.GetWorldObjectCountInNGrid<Player>() && !map.ActiveObjectsNearGrid(grid))
        {
            ObjectGridStoper worker;
            TypeContainerVisitor<ObjectGridStoper, GridTypeMapContainer> visitor(worker);
            grid.VisitAllGrids(visitor);
            grid.SetGridState(GRID_STATE_IDLE);
            SF_LOG_DEBUG("maps", "Grid[%u, %u] on map %u moved to IDLE state", grid.getX(), grid.getY(), map.GetId());
        }
        else
            map.ResetGridExpiry(grid, 0.1f);
    }
}

void IdleState::Update(Map& map, NGridType& grid, GridInfo&, uint32) const
{
    map.ResetGridExpiry(grid);
    grid.SetGridState(GRID_STATE_REMOVAL);
    SF_LOG_DEBUG("maps", "Grid[%u, %u] on map %u moved to REMOVAL state", grid.getX(), grid.getY(), map.GetId());
}

void RemovalState::Update(Map& map, NGridType& grid, GridInfo& info, uint32 diff) const
{
    if (!info.getUnloadLock())
    {
        info.UpdateTimeTracker(diff);
        if (info.getTimeTracker().Passed() && !map.UnloadGrid(grid, false))
        {
            SF_LOG_DEBUG("maps", "Grid[%u, %u] for map %u differed unloading due to players or active objects nearby", grid.getX(), grid.getY(), map.GetId());
            map.ResetGridExpiry(grid);
        }
    }
}
