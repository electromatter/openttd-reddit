/* $Id$ */

/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file script_sign.cpp Implementation of ScriptSign. */

#include "../../stdafx.h"
#include "script_sign.hpp"
#include "table/strings.h"
#include "../script_instance.hpp"
#include "../../command_func.h"
#include "../../core/alloc_func.hpp"
#include "../../signs_base.h"
#include "../../string_func.h"
#include "../../strings_func.h"
#include "../../tile_map.h"
#include "../../company_func.h"

/* static */ bool ScriptSign::IsValidSign(SignID sign_id)
{
	const Sign *si = ::Sign::GetIfValid(sign_id);
	return si != NULL && si->owner == _current_company;
}

/* static */ bool ScriptSign::SetName(SignID sign_id, const char *name)
{
	EnforcePrecondition(false, IsValidSign(sign_id));
	EnforcePrecondition(false, !::StrEmpty(name));
	EnforcePreconditionCustomError(false, ::Utf8StringLength(name) < MAX_LENGTH_SIGN_NAME_CHARS, ScriptError::ERR_PRECONDITION_STRING_TOO_LONG);

	return ScriptObject::DoCommand(0, sign_id, 0, CMD_RENAME_SIGN, name);
}

/* static */ char *ScriptSign::GetName(SignID sign_id)
{
	if (!IsValidSign(sign_id)) return NULL;

	static const int len = 64;
	char *sign_name = MallocT<char>(len);

	::SetDParam(0, sign_id);
	::GetString(sign_name, STR_SIGN_NAME, &sign_name[len - 1]);

	return sign_name;
}

/* static */ TileIndex ScriptSign::GetLocation(SignID sign_id)
{
	if (!IsValidSign(sign_id)) return INVALID_TILE;

	const Sign *sign = ::Sign::Get(sign_id);
	return ::TileVirtXY(sign->x, sign->y);
}

/* static */ bool ScriptSign::RemoveSign(SignID sign_id)
{
	EnforcePrecondition(false, IsValidSign(sign_id));
	return ScriptObject::DoCommand(0, sign_id, 0, CMD_RENAME_SIGN, "");
}

/* static */ SignID ScriptSign::BuildSign(TileIndex location, const char *text)
{
	EnforcePrecondition(INVALID_SIGN, ::IsValidTile(location));
	EnforcePrecondition(INVALID_SIGN, !::StrEmpty(text));
	EnforcePreconditionCustomError(false, ::Utf8StringLength(text) < MAX_LENGTH_SIGN_NAME_CHARS, ScriptError::ERR_PRECONDITION_STRING_TOO_LONG);

	if (!ScriptObject::DoCommand(location, 0, 0, CMD_PLACE_SIGN, text, &ScriptInstance::DoCommandReturnSignID)) return INVALID_SIGN;

	/* In case of test-mode, we return SignID 0 */
	return 0;
}