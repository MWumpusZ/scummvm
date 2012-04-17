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

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef LILLIPUT_SCRIPT_H
#define LILLIPUT_SCRIPT_H

#include "common/memstream.h"
#include "common/stack.h"
#include "common/random.h"

namespace Lilliput {

class LilliputEngine;
class LilliputScript {
public:
	byte _byte16F08;

	byte _array10B29[40];
	byte _array122E9[20];
	byte _array16123[40];
	byte _array1614B[40];
	byte _array16173[40];

	int _word12A00;
	int _word12A02;
	byte _byte12A04;

	LilliputScript(LilliputEngine *vm);
	~LilliputScript();

	void runScript(Common::MemoryReadStream script);
	void runMenuScript(Common::MemoryReadStream script);
private:
	LilliputEngine *_vm;

	Common::MemoryReadStream *_currScript;
	Common::Stack<Common::MemoryReadStream *> _scriptStack;

	byte _byte16F05_ScriptHandler;
	byte _byte12A09;
	byte _byte129A0;
	byte _byte15FFA;
	byte _byte1855D;
	byte _byte10806;
	byte _byte12FE4;
	byte _byte16F02;

	byte *_savedBuffer215Ptr;
	byte _array122FD[20];
	byte _array128EF[40];
	byte _array10AB1[40];
	byte _array12811[40];
	byte _array12839[40];
	byte _array10B51[40 * 40];

	int _array12311[640];

	int _word1881B;
	int _word16F00;
	int _word10802;
	int _word10804;
	int _word15FFB;
	int _word15FFD;
	int _word1855E;
	int _word18776;

	int handleOpcode(Common::MemoryReadStream *script);
	byte handleOpcodeType1(int curWord);
	void handleOpcodeType2(int curWord);

	void sub185ED(byte index, byte subIndex);
	void sub185B4_display();
	void sub1823E(byte var1, byte var2, byte *curBufPtr);
	void sub17B6C(int var1);
	void sub16C86(int index, byte *buf);
	void sub16C5C(int index, byte var3);
	int sub17D40(int var);
	void sub18A56(byte *buf);
	void sub18B3C(int var);

	int getValue1();
	int getValue2();

	void sub130B6();

	byte *getBuffer215Ptr();
	byte compareValues(byte var1, int oper, int var2);
	void computeOperation(byte *bufPtr, int oper, int var2);

	//Opcodes Type 1
	byte OC_sub173DF();
	byte OC_sub173F0();
	byte OC_sub1740A();
	byte OC_sub17434();
	byte OC_sub17468();
	byte OC_getRandom();
	byte OC_sub1748C();
	byte OC_compWord18776();
	byte OC_checkSaveFlag();
	byte OC_sub174C8();
	byte OC_sub174D8();
	byte OC_sub1750E();
	byte OC_compareCoords_1();
	byte OC_compareCoords_2();
	byte OC_sub1757C();
	byte OC_sub1759E();
	byte OC_compWord16EF8();
	byte OC_sub175C8();
	byte OC_sub17640();
	byte OC_sub176C4();
	byte OC_compWord10804();
	byte OC_sub17766();
	byte OC_sub17782();
	byte OC_sub1779E();
	byte OC_sub177C6();
	byte OC_compWord16EFE();
	byte OC_sub177F5();
	byte OC_sub17812();
	byte OC_sub17825();
	byte OC_sub17844();
	byte OC_sub1785C();
	byte OC_sub17886();
	byte OC_sub178A8();
	byte OC_sub178BA();
	byte OC_sub178C2();
	byte OC_sub178D2();
	byte OC_sub178E8();
	byte OC_sub178FC();
	byte OC_sub1790F();
	byte OC_sub1792A();
	byte OC_sub1793E();
	byte OC_sub1795E();
	byte OC_sub1796E();
	byte OC_sub17984();
	byte OC_checkSavedMousePos();
	byte OC_sub179AE();
	byte OC_sub179C2();
	byte OC_sub179E5();
	byte OC_sub17A07();
	byte OC_sub17757();

	// Opcodes Type 2
	void OC_setWord18821();
	void OC_sub17A3E();
	void OC_sub17D57();
	void OC_sub17D7F();
	void OC_sub17DB9();
	void OC_sub17DF9();
	void OC_sub17E07();
	void OC_sub17E15();
	void OC_sub17B03();
	void OC_getRandom_type2();
	void OC_sub17A66();
	void OC_sub17A8D();
	void OC_saveAndQuit();
	void OC_sub17B93();
	void OC_sub17E37();
	void OC_resetByte1714E();
	void OC_deleteSavegameAndQuit();
	void OC_incByte16F04();
	void OC_sub17BA5();
	void OC_setByte18823();
	void OC_sub17BB7();
	void OC_sub17BF2();
	void OC_sub17ACC();
	void OC_resetByte16F04();
	void OC_sub17AE1();
	void OC_sub17AEE();
	void OC_setWord10804();
	void OC_sub17C0E();
	void OC_sub17C55();
	void OC_sub17C76();
	void OC_sub17AFC();
	void OC_sub17C8B();
	void OC_sub17CA2();
	void OC_sub17CB9();
	void OC_sub17CD1();
	void OC_resetWord16EFE();
	void OC_sub17CEF();
	void OC_sub17D1B();
	void OC_sub17D23();
	void OC_sub17E6D();
	void OC_sub17E7E();
	void OC_sub17E99();
	void OC_sub17EC5();
	void OC_sub17EF4();
	void OC_sub17F08();
	void OC_sub17F4F();
	void OC_sub17F68();
	void OC_getNextVal();
	void OC_sub17FD2();
	void OC_sub17FDD();
	void OC_setByte10B29();
	void OC_sub18007();
	void OC_sub18014();
	void OC_sub1801D();
	void OC_sub1805D();
	void OC_sub18074();
	void OC_sub1808B();
	void OC_sub18099();
	void OC_sub180C3();
	void OC_sub1810A();
	void OC_sub1812D();
	void OC_sub1817F();
	void OC_sub181BB();
	void OC_sub18213();
	void OC_sub18252();
	void OC_sub18260();
	void OC_sub182EC();
	void OC_unkPaletteFunction_1();
	void OC_unkPaletteFunction_2();
	void OC_loadAndDisplayCUBESx_GFX();
	void OC_sub1834C();
	void OC_sub18359();
	void OC_sub18367();
	void OC_sub17D04();
	void OC_sub18387();
	void OC_setByte14835();
	void OC_setByte14837();
	void OC_sub183A2();
	void OC_sub183C6();
	void OC_loadFile_AERIAL_GFX();
	void OC_sub17E22();
	void OC_sub1844A();
	void OC_sub1847F();
	void OC_displayVGAFile();
	void OC_sub184D7();
	void OC_displayTitleScreen();
	void OC_sub1853B();
	void OC_sub1864D();
	void OC_initArr18560();
	void OC_sub18678();
	void OC_sub18690();
	void OC_setWord10802();
	void OC_sub186A1();
	void OC_sub186E5_snd();
	void OC_sub1870A_snd();
	void OC_sub18725_snd();
	void OC_sub18733_snd();
	void OC_sub1873F_snd();
	void OC_sub18746_snd();
	void OC_sub1875D_snd();
	void OC_sub18764();
};

} // End of namespace Lilliput

#endif
