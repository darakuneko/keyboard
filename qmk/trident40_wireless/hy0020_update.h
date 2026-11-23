// Copyright 2026 Daraku-Neko (@Daraku-Neko)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <stdbool.h>

bool hy0020_update_active(void);
void hy0020_update_start(void);   // BT_UPDT key, USB only
void hy0020_update_stop(void);
void hy0020_update_task(void);
