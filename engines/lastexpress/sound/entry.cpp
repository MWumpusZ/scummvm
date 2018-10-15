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

#include "lastexpress/sound/entry.h"

#include "lastexpress/game/logic.h"
#include "lastexpress/game/savepoint.h"
#include "lastexpress/game/state.h"

#include "lastexpress/sound/queue.h"
#include "lastexpress/sound/sound.h"

#include "lastexpress/graphics.h"
#include "lastexpress/lastexpress.h"
#include "lastexpress/resource.h"

namespace LastExpress {

#define SOUNDCACHE_ENTRY_SIZE 92160
#define FILTER_BUFFER_SIZE 2940

//////////////////////////////////////////////////////////////////////////
// SoundEntry
//////////////////////////////////////////////////////////////////////////
SoundEntry::SoundEntry(LastExpressEngine *engine) : _engine(engine) {
	_status = 0;
	_type = kSoundTypeNone;

	_currentDataPtr = NULL;

	_blockCount = 0;
	_time = 0;

	_stream = NULL;

	_field_34 = 0;
	_field_38 = 0;
	_field_3C = 0;
	_variant = 0;
	_entity = kEntityPlayer;
	_field_48 = 0;
	_priority = 0;

	_subtitle = NULL;

	_soundStream = NULL;

	_queued = false;
}

SoundEntry::~SoundEntry() {
	// Entries that have been queued will have their streamed disposed automatically
	if (!_soundStream)
		SAFE_DELETE(_stream);

	SAFE_DELETE(_soundStream);

	free(_currentDataPtr);

	_subtitle = NULL;
	_stream = NULL;

	// Zero passed pointers
	_engine = NULL;
}

void SoundEntry::open(Common::String name, SoundFlag flag, int priority) {
	_priority = priority;
	setType(flag);
	setupStatus(flag);
	loadStream(name);
}

void SoundEntry::close() {
	_status |= kSoundFlagCloseRequested;

	// Loop until ready
	//while (!(_status & kSoundFlagClosed) && !(getSoundQueue()->getFlag() & 8) && (getSoundQueue()->getFlag() & 1))
	//	;	// empty loop body

	// The original game remove the entry from the cache here,
	// but since we are called from within an iterator loop
	// we will remove the entry there
	// removeFromCache(entry);

	if (_subtitle) {
		_subtitle->draw();
		SAFE_DELETE(_subtitle);
	}

	if (_entity) {
		if (_entity == kEntitySteam)
			getSound()->playLoopingSound(2);
		else if (_entity != kEntityTrain)
			getSavePoints()->push(kEntityPlayer, _entity, kActionEndSound);
	}
}

void SoundEntry::play() {
	if (!_stream)
		error("[SoundEntry::play] stream has been disposed");

	// Prepare sound stream
	if (!_soundStream)
		_soundStream = new StreamedSound();

	// Compute current filter id
	int32 filterId = _status & kSoundVolumeMask;
	// TODO adjust status (based on stepIndex)

	if (_queued) {
		_soundStream->setFilterId(filterId);
	} else {
		_stream->seek(0);

		// Load the stream and start playing
		_soundStream->load(_stream, filterId);

		_queued = true;
	}
}

bool SoundEntry::isFinished() {
	if (!_stream)
		return true;

	if (!_soundStream || !_queued)
		return false;

	// TODO check that all data has been queued
	return _soundStream->isFinished();
}

void SoundEntry::setType(SoundFlag flag) {
	switch (flag & kSoundTypeMask) {
	default:
	case kSoundTypeNormal:
		_type = getSoundQueue()->getCurrentType();
		getSoundQueue()->setCurrentType((SoundType)(_type + 1));
		break;

	case kSoundTypeAmbient: {
		SoundEntry *previous2 = getSoundQueue()->getEntry(kSoundType2);
		if (previous2)
			previous2->update(0);

		SoundEntry *previous = getSoundQueue()->getEntry(kSoundType1);
		if (previous) {
			previous->setType(kSoundType2);
			previous->update(0);
		}

		_type = kSoundType1;
		}
		break;

	case kSoundTypeWalla: {
		SoundEntry *previous = getSoundQueue()->getEntry(kSoundType3);
		if (previous) {
			previous->setType(kSoundType4);
			previous->update(0);
		}

		_type = kSoundType11;
		}
		break;

	case kSoundTypeLink: {
		SoundEntry *previous = getSoundQueue()->getEntry(kSoundType7);
		if (previous)
			previous->setType(kSoundType8);

		_type = kSoundType7;
		}
		break;

	case kSoundTypeNIS: {
		SoundEntry *previous = getSoundQueue()->getEntry(kSoundType9);
		if (previous)
			previous->setType(kSoundType10);

		_type = kSoundType9;
		}
		break;

	case kSoundTypeIntro: {
		SoundEntry *previous = getSoundQueue()->getEntry(kSoundType11);
		if (previous)
			previous->setType(kSoundType14);

		_type = kSoundType11;
		}
		break;

	case kSoundTypeMenu: {
		SoundEntry *previous = getSoundQueue()->getEntry(kSoundType13);
		if (previous)
			previous->setType(kSoundType14);

		_type = kSoundType13;
		}
		break;
	}
}

void SoundEntry::setupStatus(SoundFlag flag) {
	_status = flag;
	if ((_status & kSoundVolumeMask) == kVolumeNone)
		_status |= kSoundFlagMuteRequested;

	if (!(_status & kSoundFlagLooped))
		_status |= kSoundFlagCloseOnDataEnd;
}

void SoundEntry::loadStream(Common::String name) {
	_name2 = name;

	// Load sound data
	_stream = getArchive(name);

	if (!_stream)
		_stream = getArchive("DEFAULT.SND");

	if (!_stream)
		_status = kSoundFlagCloseRequested;
}

void SoundEntry::update(uint val) {
	if (_status & kSoundFlagFading)
		return;

	int value2 = val;

	_status |= kSoundFlagVolumeChanging;

	if (val) {
		if (getSoundQueue()->getFlag() & 32) {
			_variant = val;
			value2 = val * 2 + 1;
		}

		_field_3C = value2;
	} else {
		_field_3C = 0;
		_status |= kSoundFlagFading;
	}
}

bool SoundEntry::updateSound() {
	assert(_name2.size() <= 16);

	bool result;
	char sub[16];

	if (_status & kSoundFlagClosed) {
		result = false;
	} else {
		if (_status & kSoundFlagDelayedActivate) {
			if (_field_48 <= getSound()->getData2()) {
				_status |= kSoundFlagPlayRequested;
				_status &= ~kSoundFlagDelayedActivate;
				strcpy(sub, _name2.c_str());

				// FIXME: Rewrite and document expected behavior
				int l = strlen(sub) + 1;
				if (l - 1 > 4)
					sub[l - (1 + 4)] = 0;
				showSubtitle(sub);
			}
		} else {
			if (!(getSoundQueue()->getFlag() & 0x20)) {
				if (!(_status & kSoundFlagFixedVolume)) {
					if (_entity) {
						if (_entity < 0x80) {
							updateEntryFlag(getSound()->getSoundFlag(_entity));
						}
					}
				}
			}
			//if (_status & kSoundFlagHasUnreadData && !(_status & kSoundFlagMute) && v1->soundBuffer)
			//	Sound_FillSoundBuffer(v1);
		}
		result = true;
	}

	return result;
}

void SoundEntry::updateEntryFlag(SoundFlag flag) {
	if (flag) {
		if (getSoundQueue()->getFlag() & 0x20 && _type != kSoundType9 && _type != kSoundType7)
			update(flag);
		else
			_status = flag + (_status & ~kSoundVolumeMask);
	} else {
		_variant = 0;
		_status |= kSoundFlagMuteRequested;
		_status &= ~(kSoundFlagVolumeChanging | kSoundVolumeMask);
	}
}

void SoundEntry::updateState() {
	if (getSoundQueue()->getFlag() & 32) {
		if (_type != kSoundType9 && _type != kSoundType7 && _type != kSoundType5) {
			uint32 variant = _status & kSoundVolumeMask;

			_status &= ~kSoundVolumeMask;

			_variant = variant;
			_status |= variant * 2 + 1;
		}
	}

	_status |= kSoundFlagPlayRequested;
}

void SoundEntry::reset() {
	_status |= kSoundFlagCloseRequested;
	_entity = kEntityPlayer;

	if (_stream) {
		if (!_soundStream) {
			SAFE_DELETE(_stream);
		} else {
			// the original stream will be disposed
			_soundStream->stop();
			SAFE_DELETE(_soundStream);
		}

		_stream = NULL;
	}
}

void SoundEntry::showSubtitle(Common::String filename) {
	_subtitle = new SubtitleEntry(_engine);
	_subtitle->load(filename, this);

	if (_subtitle->getStatus() & 0x400) {
		_subtitle->draw();
		SAFE_DELETE(_subtitle);
	} else {
		_status |= kSoundFlagHasSubtitles;
	}
}

void SoundEntry::saveLoadWithSerializer(Common::Serializer &s) {
	assert(_name1.size() <= 16);
	assert(_name2.size() <= 16);

	if (_name2.matchString("NISSND?") && ((_status & kSoundTypeMask) != kSoundTypeMenu)) {
		s.syncAsUint32LE(_status);
		s.syncAsUint32LE(_type);
		s.syncAsUint32LE(_blockCount); // field_8;
		s.syncAsUint32LE(_time);
		s.syncAsUint32LE(_field_34); // field_10;
		s.syncAsUint32LE(_field_38); // field_14;
		s.syncAsUint32LE(_entity);

		uint32 delta = (uint32)_field_48 - getSound()->getData2();
		if (delta > 0x8000000u) // sanity check against overflow
			delta = 0;
		s.syncAsUint32LE(delta);

		s.syncAsUint32LE(_priority);

		char name1[16];
		strcpy((char *)&name1, _name1.c_str());
		s.syncBytes((byte *)&name1, 16);

		char name2[16];
		strcpy((char *)&name2, _name2.c_str());
		s.syncBytes((byte *)&name2, 16);
	}
}

//////////////////////////////////////////////////////////////////////////
// SubtitleEntry
//////////////////////////////////////////////////////////////////////////
SubtitleEntry::SubtitleEntry(LastExpressEngine *engine) : _engine(engine) {
	_status = 0;
	_sound = NULL;
	_data = NULL;
}

SubtitleEntry::~SubtitleEntry() {
	SAFE_DELETE(_data);

	// Zero-out passed pointers
	_sound = NULL;
	_engine = NULL;
}

void SubtitleEntry::load(Common::String filename, SoundEntry *soundEntry) {
	// Add ourselves to the list of active subtitles
	getSoundQueue()->addSubtitle(this);

	// Set sound entry and filename
	_filename = filename + ".SBE";
	_sound = soundEntry;

	// Load subtitle data
	if (_engine->getResourceManager()->hasFile(_filename)) {
		if (getSoundQueue()->getSubtitleFlag() & 2)
			return;

		loadData();
	} else {
		_status = kSoundFlagClosed;
	}
}

void SubtitleEntry::loadData() {
	_data = new SubtitleManager(_engine->getFont());
	_data->load(getArchive(_filename));

	getSoundQueue()->setSubtitleFlag(getSoundQueue()->getSubtitleFlag() | 2);
	getSoundQueue()->setCurrentSubtitle(this);
}

void SubtitleEntry::setupAndDraw() {
	if (!_sound)
		error("[SubtitleEntry::setupAndDraw] Sound entry not initialized");

	if (!_data) {
		_data = new SubtitleManager(_engine->getFont());
		_data->load(getArchive(_filename));
	}

	if (_data->getMaxTime() > _sound->getTime()) {
		_status = kSoundFlagClosed;
	} else {
		_data->setTime((uint16)_sound->getTime());

		if (getSoundQueue()->getSubtitleFlag() & 1)
			drawOnScreen();
	}

	getSoundQueue()->setCurrentSubtitle(this);

	// TODO Missing code
}

void SubtitleEntry::draw() {
	// Remove ourselves from the queue
	getSoundQueue()->removeSubtitle(this);

	if (this == getSoundQueue()->getCurrentSubtitle()) {
		drawOnScreen();

		getSoundQueue()->setCurrentSubtitle(NULL);
		getSoundQueue()->setSubtitleFlag(0);
	}
}

void SubtitleEntry::drawOnScreen() {
	if (_data == NULL)
		return;

	getSoundQueue()->setSubtitleFlag(getSoundQueue()->getSubtitleFlag() & -2);
	_engine->getGraphicsManager()->draw(_data, GraphicsManager::kBackgroundOverlay);
}

} // End of namespace LastExpress
