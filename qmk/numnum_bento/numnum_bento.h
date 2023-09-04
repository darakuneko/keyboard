#pragma once

#define LAYOUT( \
    S01, S02, S03, S04, S05, \
    S06, S07, S08, S09, S10, \
    S11, S12, S13, S14,  \
    r01, r02, r03, r04, r05, \
    t01, t02, t03, t04, t05, t06, t07, t08, t09 \
) \
{ \
    { r01,   r02,   r03,   r04,  KC_NO, S01,   S02,   S03,   S04,   S05,   }, \
    { t01,   t02,   t03,   t04,  t05,   S06,   S07,   S08,   S09,   S10,   }, \
    { t06,   t07,   t08,   t09,  r05,   S11,   S12,   S13,   S14,   KC_NO  } \
}
