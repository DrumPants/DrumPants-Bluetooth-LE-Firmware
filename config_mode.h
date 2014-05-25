/*
 * config_mode.h
 *
 *  Created on: May 13, 2014
 *      Author: tyler
 */

#ifndef CONFIG_MODE_H_
#define CONFIG_MODE_H_

#define CONFIG_COMMAND_START_BYTE 0xB0

enum {
	CONFIG_COMMAND_NOOP,
	CONFIG_COMMAND_REPORT_VERSION
} ConfigCommands;

#endif /* CONFIG_MODE_H_ */
