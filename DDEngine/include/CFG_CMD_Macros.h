#pragma once

#include "DDEUtils.h"

#define CFG_ARG(X) (tokens.size() > X + 1) ? tokens.at(X + 1) : "" 
#define CFG_ARG_BOOL(X) StringUtils::toBool(CFG_ARG(X))
#define CFG_ARG_FLOAT(X) StringUtils::toFloat(CFG_ARG(X))
#define CFG_ARG_INT(X) StringUtils::toInt(CFG_ARG(X))
#define CFG_LOAD(CMD) if(tokens.at(0) == CMD)

#define CMD_ARG(X) (tokens.size() > X + 1) ? tokens.at(X + 1) : "" 
#define CMD_ARG_BOOL(X) StringUtils::toBool(CMD_ARG(X))
#define CMD_ARG_FLOAT(X) StringUtils::toFloat(CMD_ARG(X))
#define CMD_ARG_INT(X) StringUtils::toInt(CMD_ARG(X))
#define CMD_EXECUTE(CMD) if(tokens.at(0) == CMD)
#define CMD_INFO(X, T) getHUD().notification(X, T);