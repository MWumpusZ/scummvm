/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef MUTATIONOFJB_ROOM_H
#define MUTATIONOFJB_ROOM_H

#include "common/scummsys.h"
#include "common/array.h"
#include "graphics/surface.h"
#include "graphics/managed_surface.h"

namespace Graphics {
class Screen;
}

namespace MutationOfJB {

class EncryptedFile;
class Game;

class Room {
public:
	friend class RoomAnimationDecoderCallback;
	friend class GuiAnimationDecoderCallback;

	Room(Game *game, Graphics::Screen *screen);
	bool load(uint8 roomNumber, bool roomB);
	void drawObjectAnimation(uint8 objectId, int animOffset);
	void drawBitmap(uint8 bitmapId);
	void drawFrames(uint8 fromFrame, uint8 toFrame, const Common::Rect &area = Common::Rect(), uint8 threshold = 0xFF);
	void redraw();
private:
	Game *_game;
	Graphics::Screen *_screen;
	Graphics::ManagedSurface _background;
	Common::Array<Graphics::Surface> _surfaces;
	Common::Array<int> _objectsStart;
};

}

#endif
