/* SPDX-License-Identifier: GPL-2.0-or-later */

#pragma once

#include "quantum.h"

#define XXX KC_NO

#define LAYOUT( \
	k0A, k0B, k0C, k0D, k0E, k0F, k0G, k0H, k0I, k0J, k0K, k0L, k0M, k0N, k0O, k0P, k1A, k1B, k1C, k1D, k1E, k1F, k1G, k1H, k1I, k1J, k1K, k1L, k1M, k1N, k1O, k2A, k2B, k2C, k2D, k2E, k2F, k2G, k2H, k2I, k2J, k2K, k2L, k2M, k2N, k2O, k3A, k3B, k3C, k3D, k3E, k3F, k3G, k3H, k3I, k3J, k3K, k3L, k3M, k3N, k4A, k4B, k4C, k4D, k4E, k4F, k4G, k4H, k4I, k4J, k4K, k4L, k4N, k5A, k5C, k5E, k5F, k5G, k5H, k5I, k5K, k4M, k5L, k5M, k5N \
) { \
	{k0A, k0B, k0C, k0D, k0E, k0F, k0G, k0H, k0I, k0J, k0K, k0L, k0M, k0N, k0O, k0P}, \
	{k1A, k1B, k1C, k1D, k1E, k1F, k1G, k1H, k1I, k1J, k1K, k1L, k1M, k1N, k1O, XXX}, \
	{k2A, k2B, k2C, k2D, k2E, k2F, k2G, k2H, k2I, k2J, k2K, k2L, k2M, k2N, k2O, XXX}, \
	{k3A, k3B, k3C, k3D, k3E, k3F, k3G, k3H, k3I, k3J, k3K, k3L, k3M, k3N, XXX, XXX}, \
	{k4A, k4B, k4C, k4D, k4E, k4F, k4G, k4H, k4I, k4J, k4K, k4L, k4M, k4N, XXX, XXX}, \
	{k5A, XXX, k5C, XXX, k5E, k5F, k5G, k5H, k5I, XXX, k5K, k5L, k5M, k5N, XXX, XXX} \
}
