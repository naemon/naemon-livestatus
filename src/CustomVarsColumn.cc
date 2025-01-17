// Copyright (c) 2025 Naemon team - license: GPL-2.0
// This file is part of the naemon project: https://www.naemon.io
// See LICENSE file in the project root for details.

#include "CustomVarsColumn.h"
#include "nagios.h"
#include "logger.h"
#include "CustomVarsFilter.h"
#include "Query.h"
#include "auth.h"
#include <string.h>

void CustomVarsColumn::output(void *data, Query *query)
{

	if (_what == CVT_DICT)
		query->outputBeginDict();
	else
		query->outputBeginList();

	customvariablesmember *cvm = getCVM(data);

	bool first = true;
	while (cvm) {
		if(g_hidden_custom_var_prefix[0]==0 || 0!=strncmp(g_hidden_custom_var_prefix, cvm->variable_name, strlen(g_hidden_custom_var_prefix))) {
			if (first)
				first = false;
			else if (_what == CVT_DICT)
				query->outputDictSeparator();
			else
				query->outputListSeparator();
			if (_what == CVT_VARNAMES)
				query->outputString(cvm->variable_name);
			else if (_what == CVT_VALUES)
				query->outputString(cvm->variable_value);
			else {
				query->outputString(cvm->variable_name);
				query->outputDictValueSeparator();
				query->outputString(cvm->variable_value);
			}
		}
		cvm = cvm->next;
	}

	if (_what == CVT_DICT)
		query->outputEndDict();
	else
		query->outputEndList();
}

Filter *CustomVarsColumn::createFilter(int opid, char *value)
{
	return new CustomVarsFilter(this, opid, value);
}


customvariablesmember *CustomVarsColumn::getCVM(void *data)
{
	if (!data) return 0;
	data = shiftPointer(data);
	if (!data) return 0;
	return *(customvariablesmember **)((char *)data + _offset);
}


bool CustomVarsColumn::contains(void *data, const char *value)
{
	customvariablesmember *cvm = getCVM(data);
	while (cvm) {
		if(g_hidden_custom_var_prefix[0]==0 || 0!=strncmp(g_hidden_custom_var_prefix, cvm->variable_name, strlen(g_hidden_custom_var_prefix))) {
			char *ref = _what == CVT_VARNAMES ? cvm->variable_name : cvm->variable_value;
			if (!strcmp(ref, value))
				return true;
		}
		cvm = cvm->next;
	}
	return false;
}

char *CustomVarsColumn::getVariable(void *data, const char *varname)
{
	customvariablesmember *cvm = getCVM(data);
	while (cvm) {
		if(g_hidden_custom_var_prefix[0]==0 || 0!=strncmp(g_hidden_custom_var_prefix, cvm->variable_name, strlen(g_hidden_custom_var_prefix))) {
			if (!strcmp(cvm->variable_name, varname))
				return cvm->variable_value;
		}
		cvm = cvm->next;
	}
	return 0;
}
