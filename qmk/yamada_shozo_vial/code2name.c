#include <stdio.h>
#include <stdint.h>
#include <string.h>

char key_name[20];

struct Code2Name {
    char* key;
    uint16_t value;
};

struct Code2Name kbName1[] = {
	{"NO", 0x00},
	{"TRNS", 0x01},
	{"A", 0x04},
	{"B", 0x05},
	{"C", 0x06},
	{"D", 0x07},
	{"E", 0x08},
	{"F", 0x09},
	{"G", 0x0A},
	{"H", 0x0B},
	{"I", 0x0C},
	{"J", 0x0D},
	{"K", 0x0E},
	{"L", 0x0F},
	{"M", 0x10},
	{"N", 0x11},
	{"O", 0x12},
	{"P", 0x13},
	{"Q", 0x14},
	{"R", 0x15},
	{"S", 0x16},
	{"T", 0x17},
	{"U", 0x18},
	{"V", 0x19},
	{"W", 0x1A},
	{"X", 0x1B},
	{"Y", 0x1C},
	{"Z", 0x1D},
	{"1", 0x1E},
	{"2", 0x1F},
	{"3", 0x20},
	{"4", 0x21},
	{"5", 0x22},
	{"6", 0x23},
	{"7", 0x24},
	{"8", 0x25},
	{"9", 0x26},
	{"0", 0x27},
	{"ENTER", 0x28},
	{"ESC", 0x29},
	{"BSAPCE", 0x2A},
	{"TAB", 0x2B},
	{"SPACE", 0x2C},
	{"MINUS", 0x2D},
	{"EQUAL", 0x2E},
	{"LBRCKT", 0x2F},
	{"RBRCKT", 0x30},
	{"BSLASH", 0x31},
	{"NNS_HASH", 0x32},
	{"SCLN", 0x33},
	{"QUOTE", 0x34},
	{"GRAVE", 0x35},
	{"COMMA", 0x36},
	{"DOT", 0x37},
	{"SLASH", 0x38},
	{"CPSL", 0x39},
	{"F1", 0x3A},
	{"F2", 0x3B},
	{"F3", 0x3C},
	{"F4", 0x3D},
	{"F5", 0x3E},
	{"F6", 0x3F},
	{"F7", 0x40},
	{"F8", 0x41},
	{"F9", 0x42},
	{"F10", 0x43},
	{"F11", 0x44},
	{"F12", 0x45},
	{"PSCREEN", 0x46},
	{"SCRLK", 0x47},
	{"PAUSE", 0x48},
	{"INSERT", 0x49},
	{"HOME", 0x4A},
	{"PGUP", 0x4B},
	{"DEL", 0x4C},
	{"END", 0x4D},
	{"PGDOWN", 0x4E},
	{"RIGHT", 0x4F},
	{"LEFT", 0x50},
	{"DOWN", 0x51},
	{"UP", 0x52},
	{"NUMLCK", 0x53},
	{"KP_SLASH", 0x54},
	{"KP_ASTR", 0x55},
	{"KP_MINUS", 0x56},
	{"KP_PLUS", 0x57},
	{"KP_ENTER", 0x58},
	{"KP_1", 0x59},
	{"KP_2", 0x5A},
	{"KP_3", 0x5B},
	{"KP_4", 0x5C},
	{"KP_5", 0x5D},
	{"KP_6", 0x5E},
	{"KP_7", 0x5F},
	{"KP_8", 0x60},
	{"KP_9", 0x61},
	{"KP_0", 0x62},
	{"KP_DOT", 0x63},
	{"NNS_BSLSH", 0x64},
	{"APP", 0x65},
	{"KP_EQUAL", 0x67},
	{"F13", 0x68},
	{"F14", 0x69},
	{"F15", 0x6A},
	{"F16", 0x6B},
	{"F17", 0x6C},
	{"F18", 0x6D},
	{"F19", 0x6E},
	{"F20", 0x6F},
	{"F21", 0x70},
	{"F22", 0x71},
	{"F23", 0x72},
	{"F24", 0x73},
	{"EXEC", 0x74},
	{"HELP", 0x75},
	{"SLCT", 0x77},
	{"STOP", 0x78},
	{"AGIN", 0x79},
	{"UNDO", 0x7A},
	{"CUT", 0x7B},
	{"COPY", 0x7C},
	{"PSTE", 0x7D},
	{"FIND", 0x7E},
	{"VOLUP", 0x80},
	{"LSFT", 0x80},
	{"VOLDOWN", 0x81},
	{"LCAP", 0x82},
	{"LNUM", 0x83},
	{"LSCR", 0x84},
	{"KP_COMMA", 0x85},
	{"RO", 0x87},
	{"KANA", 0x88},
	{"JYEN", 0x89},
	{"HENK", 0x8A},
	{"MHEN", 0x8B},
	{"LANG1", 0x90},
	{"LANG2", 0x91},
	{"PWR", 0xA5},
	{"SLEP", 0xA6},
	{"WAKE", 0xA7},
	{"MUTE", 0xA8},
	{"VOLU", 0xA9},
	{"VOLD", 0xAA},
	{"MNXT", 0xAB},
	{"MPRV", 0xAC},
	{"MSTP", 0xAD},
	{"MPLY", 0xAE},
	{"MSEL", 0xAF},
	{"EJCT", 0xB0},
	{"MAIL", 0xB1},
	{"CALC", 0xB2},
	{"MYCM", 0xB3},
	{"WSCH", 0xB4},
	{"WHOM", 0xB5},
	{"WBAK", 0xB6},
	{"WFWD", 0xB7},
	{"WSTP", 0xB8},
	{"WREF", 0xB9},
	{"WFAV", 0xBA},
	{"MFFD", 0xBB},
	{"MRWD", 0xBC},
	{"BRIU", 0xBD},
	{"BRID", 0xBE},
	{"MS_U", 0xCD},
	{"MS_D", 0xCE},
	{"MS_L", 0xCF},
	{"MS_R", 0xD0},
	{"BTN1", 0xD1},
	{"BTN2", 0xD2},
	{"BTN3", 0xD3},
	{"BTN4", 0xD4},
	{"BTN5", 0xD5},
	{"WH_U", 0xD9},
	{"WH_D", 0xDA},
	{"WH_L", 0xDB},
	{"WH_R", 0xDC},
	{"ACL0", 0xDD},
	{"ACL1", 0xDE},
	{"ACL2", 0xDF},
	{"LCTRL", 0xE0},
	{"LSHIFT", 0xE1},
	{"LALT", 0xE2},
	{"LGUI", 0xE3},
	{"RCTRL", 0xE4},
	{"RSHIFT", 0xE5},
	{"RALT", 0xE6},
	{"RGUI", 0xE7},
};
int numElm1 = sizeof(kbName1) / sizeof(kbName1[0]);

struct Code2Name kbName2[] = {
	{"EXLM", 0x21E},
	{"AT", 0x21F},
	{"HASH", 0x220},
	{"DLR", 0x221},
	{"PERC", 0x222},
	{"CIRC", 0x223},
	{"AMPR", 0x224},
	{"ASTR", 0x225},
	{"LPRN", 0x226},
	{"RPRN", 0x227},
	{"UNDS", 0x22D},
	{"PLUS", 0x22E},
	{"LCBR", 0x22F},
	{"RCBR", 0x230},
	{"PIPE", 0x231},
	{"COLN", 0x233},
	{"DQUO", 0x234},
	{"TILD", 0x235},
	{"LT", 0x236},
	{"GT", 0x237},
	{"QUES", 0x238},
};
int numElm2 = sizeof(kbName2) / sizeof(kbName2[0]);

struct Code2Name kbName3[] = {
	{"LCTL", 0x100},
	{"LSFT", 0x200},
	{"C_S", 0x300},
	{"LALT", 0x400},
	{"LCA", 0x500},
	{"LSA", 0x600},
	{"MEH", 0x700},
	{"LGUI", 0x800},
	{"LCG", 0x900},
	{"SGUI", 0xA00},
	{"LCAG", 0xD00},
	{"HYPR", 0xF00},
	{"RCTL", 0x1100},
	{"RSFT", 0x1200},
	{"RALT", 0x1400},
	{"RGUI", 0x1800},
	{"RCG", 0x1900},
  {"QK_MOD_TAP", 0x2000},
	{"LCTL_T", 0x2100},
	{"LSFT_T", 0x2200},
	{"C_S_T", 0x2300},
	{"LALT_T", 0x2400},
	{"LCA_T", 0x2500},
	{"LSA_T", 0x2600},
	{"MEH_T", 0x2700},
	{"LGUI_T", 0x2800},
	{"LCG_T", 0x2900},
	{"SGUI_T", 0x2A00},
	{"LCAG_T", 0x2D00},
	{"ALL_T", 0x2F00},
	{"RCTL_T", 0x3100},
	{"RSFT_T", 0x3200},
	{"RALT_T", 0x3400},
	{"RGUI_T", 0x3800},
	{"RCG_T", 0x3900},
	{"RCAG_T", 0x3D00},
	{"QK_LAYER_TAP", 0x4000},
	{"QK_LAYER_MOD", 0x5000},
	{"QK_TO", 0x5200},
	{"QK_MOMENTARY", 0x5220},
	{"QK_DEF_LAYER", 0x5240},
	{"QK_TOGGLE_LAYER", 0x5260},
	{"QK_ONE_SHOT_LAYER", 0x5280},
	{"QK_ONE_SHOT_MOD", 0x52A0},
	{"QK_LAYER_TAP_TOGGLE", 0x52C0},
	{"QK_TAP_DANCE", 0x5700},
	{"QK_MACRO", 0x7700},
  {"QK_KB", 0x7E00}
};
int numElm3 = sizeof(kbName3) / sizeof(kbName3[0]);

struct Code2Name kbName4[] = {
	{"OSM(MOD_LCTL)", 0x52A1},
	{"OSM(MOD_LSFT)", 0x52A2},
	{"OSM(LC|LS)", 0x52A3},
	{"OSM(MOD_LALT)", 0x52A4},
	{"OSM(LC|LA)", 0x52A5},
	{"OSM(MLS|LA)", 0x52A6},
	{"OSM(MOD_MEH)", 0x52A7},
	{"OSM(MOD_LGUI)", 0x52A8},
	{"OSM(LC|MLG)", 0x52A9},
	{"OSM(MLS|MLG)", 0x52AA},
	{"OSM(LC|LS|MLG)", 0x52AB},
	{"OSM(LA|MLG)", 0x52AC},
	{"OSM(LC|LA|MLG)", 0x52AD},
	{"OSM(MLS|LA|MLG)", 0x52AE},
	{"OSM(MOD_HYPR)", 0x52AF},
	{"OSM(MOD_RCTL)", 0x52B1},
	{"OSM(MOD_RSFT)", 0x52B2},
	{"OSM(RC|RS)", 0x52B3},
	{"OSM(MOD_RALT)", 0x52B4},
	{"OSM(RC|RA)", 0x52B5},
	{"OSM(RS|RA)", 0x52B6},
	{"OSM(RC|RS|RA)", 0x52B7},
	{"OSM(MOD_RGUI)", 0x52B8},
	{"OSM(RC|RG)", 0x52B9},
	{"OSM(RS|RG)", 0x52BA},
	{"OSM(RC|RS|RG)", 0x52BB},
	{"OSM(RA|RG)", 0x52BC},
	{"OSM(RC|RA|RG)", 0x52BD},
	{"OSM(RS|RA|RG)", 0x52BE},
	{"OSM(RC|RS|RA|RG)", 0x52BF},
	{"M_SWP_CTRL_CPS", 0x7000},
	{"M_USWP_CTRL_CPS", 0x7001},
	{"M_UCPS_TO_CTRL", 0x7003},
	{"M_CPSL_TO_CTRL", 0x7004},
	{"M_SWP_LA_LG", 0x7005},
	{"M_USWP_LA_LG", 0x7006},
	{"M_SWP_RA_RG", 0x7007},
	{"M_USWP_RA_RG", 0x7008},
	{"M_UNNO_G", 0x7009},
	{"M_NO_G", 0x700A},
	{"M_SWP_GRV_ESC", 0x700C},
	{"M_USWP_GRV_ESC", 0x700D},
	{"M_SWP_BSL_BSP", 0x700E},
	{"M_USWP_BSL_BSP", 0x700F},
	{"M_HOST_NKRO", 0x7011},
	{"M_UNHOST_NKRO", 0x7012},
	{"M_TGGL_NKRO", 0x7013},
	{"M_SWP_A_G", 0x7014},
	{"M_USWP_A_G", 0x7015},
	{"M_TGGL_A_G", 0x7016},
	{"M_SWP_LC_LG", 0x7017},
	{"M_USWP_LC_LG", 0x7018},
	{"M_SWP_RC_RG", 0x7019},
	{"M_USWP_RC_RG", 0x701A},
	{"M_SWP_CTL_G", 0x701B},
	{"M_USWP_CTL_G", 0x701C},
	{"M_TGGL_CTL_G", 0x701D},
	{"M_EE_HANDS_L", 0x701E},
  {"M_TOGGLE_A_G", 0x7016},
	{"M_EE_HANDS_R", 0x701F},
	{"MI_C", 0x7103},
	{"MI_Cs", 0x7104},
	{"MI_D", 0x7105},
	{"MI_Ds", 0x7106},
	{"MI_E", 0x7107},
	{"MI_F", 0x7108},
	{"MI_Fs", 0x7109},
	{"MI_G", 0x710A},
	{"MI_Gs", 0x710B},
	{"MI_A", 0x710C},
	{"MI_As", 0x710D},
	{"MI_B", 0x710E},
	{"MI_C_1", 0x710F},
	{"MI_Cs_1", 0x7110},
	{"MI_D_1", 0x7111},
	{"MI_Ds_1", 0x7112},
	{"MI_E_1", 0x7113},
	{"MI_F_1", 0x7114},
	{"MI_Fs_1", 0x7115},
	{"MI_G_1", 0x7116},
	{"MI_Gs_1", 0x7117},
	{"MI_A_1", 0x7118},
	{"MI_As_1", 0x7119},
	{"MI_B_1", 0x711A},
	{"MI_C_2", 0x711B},
	{"MI_Cs_2", 0x711C},
	{"MI_D_2", 0x711D},
	{"MI_Ds_2", 0x711E},
	{"MI_E_2", 0x711F},
	{"MI_F_2", 0x7120},
	{"MI_Fs_2", 0x7121},
	{"MI_G_2", 0x7122},
	{"MI_Gs_2", 0x7123},
	{"MI_A_2", 0x7124},
	{"MI_As_2", 0x7125},
	{"MI_B_2", 0x7126},
	{"MI_C_3", 0x7127},
	{"MI_Cs_3", 0x7128},
	{"MI_D_3", 0x7129},
	{"MI_Ds_3", 0x712A},
	{"MI_E_3", 0x712B},
	{"MI_F_3", 0x712C},
	{"MI_Fs_3", 0x712D},
	{"MI_G_3", 0x712E},
	{"MI_Gs_3", 0x712F},
	{"MI_A_3", 0x7130},
	{"MI_As_3", 0x7131},
	{"MI_B_3", 0x7132},
	{"MI_C_4", 0x7133},
	{"MI_Cs_4", 0x7134},
	{"MI_D_4", 0x7135},
	{"MI_Ds_4", 0x7136},
	{"MI_E_4", 0x7137},
	{"MI_F_4", 0x7138},
	{"MI_Fs_4", 0x7139},
	{"MI_G_4", 0x713A},
	{"MI_Gs_4", 0x713B},
	{"MI_A_4", 0x713C},
	{"MI_As_4", 0x713D},
	{"MI_B_4", 0x713E},
	{"MI_C_5", 0x713F},
	{"MI_Cs_5", 0x7140},
	{"MI_D_5", 0x7141},
	{"MI_Ds_5", 0x7142},
	{"MI_E_5", 0x7143},
	{"MI_F_5", 0x7144},
	{"MI_Fs_5", 0x7145},
	{"MI_G_5", 0x7146},
	{"MI_Gs_5", 0x7147},
	{"MI_A_5", 0x7148},
	{"MI_As_5", 0x7149},
	{"MI_B_5", 0x714A},
	{"MI_OCT_N2", 0x714B},
	{"MI_OCT_N1", 0x714C},
	{"MI_OCT_0", 0x714D},
	{"MI_OCT_1", 0x714E},
	{"MI_OCT_2", 0x714F},
	{"MI_OCT_3", 0x7150},
	{"MI_OCT_4", 0x7151},
	{"MI_OCT_5", 0x7152},
	{"MI_OCT_6", 0x7153},
	{"MI_OCT_7", 0x7154},
	{"MI_OCTD", 0x7155},
	{"MI_OCTU", 0x7156},
	{"MI_TRNS_N6", 0x7157},
	{"MI_TRNS_N5", 0x7158},
	{"MI_TRNS_N4", 0x7159},
	{"MI_TRNS_N3", 0x715A},
	{"MI_TRNS_N2", 0x715B},
	{"MI_TRNS_N1", 0x715C},
	{"MI_TRNS_0", 0x715D},
	{"MI_TRNS_1", 0x715E},
	{"MI_TRNS_2", 0x715F},
	{"MI_TRNS_3", 0x7160},
	{"MI_TRNS_4", 0x7161},
	{"MI_TRNS_5", 0x7162},
	{"MI_TRNS_6", 0x7163},
	{"MI_TRNSD", 0x7164},
	{"MI_TRNSU", 0x7165},
	{"MI_VEL_1", 0x7167},
	{"MI_VEL_2", 0x7168},
	{"MI_VEL_3", 0x7169},
	{"MI_VEL_4", 0x716A},
	{"MI_VEL_5", 0x716B},
	{"MI_VEL_6", 0x716C},
	{"MI_VEL_7", 0x716D},
	{"MI_VEL_8", 0x716E},
	{"MI_VEL_9", 0x716F},
	{"MI_VEL_10", 0x7170},
	{"MI_VELD", 0x7171},
	{"MI_VELU", 0x7172},
	{"MI_CH1", 0x7173},
	{"MI_CH2", 0x7174},
	{"MI_CH3", 0x7175},
	{"MI_CH4", 0x7176},
	{"MI_CH5", 0x7177},
	{"MI_CH6", 0x7178},
	{"MI_CH7", 0x7179},
	{"MI_CH8", 0x717A},
	{"MI_CH9", 0x717B},
	{"MI_CH10", 0x717C},
	{"MI_CH11", 0x717D},
	{"MI_CH12", 0x717E},
	{"MI_CH13", 0x717F},
	{"MI_CH14", 0x7180},
	{"MI_CH15", 0x7181},
	{"MI_CH16", 0x7182},
	{"MI_CHD", 0x7183},
	{"MI_CHU", 0x7184},
	{"MI_ALLOFF", 0x7185},
	{"MI_SUS", 0x7186},
	{"MI_PORT", 0x7187},
	{"MI_SOST", 0x7188},
	{"MI_SOFT", 0x7189},
	{"MI_LEG", 0x718A},
	{"MI_MOD", 0x718B},
	{"MI_MODSD", 0x718C},
	{"MI_MODSU", 0x718D},
	{"MI_BENDD", 0x718E},
	{"MI_BENDU", 0x718F},
	{"AU_ON", 0x7480},
	{"AU_OFF", 0x7481},
	{"AU_TOG", 0x7482},
	{"CLICKY_TOGGLE", 0x748A},
	{"CLICKY_UP", 0x748D},
	{"CLICKY_DOWN", 0x748E},
	{"CLICKY_RESET", 0x748F},
	{"MU_ON", 0x7490},
	{"MU_OFF", 0x7491},
	{"MU_TOG", 0x7492},
	{"MU_MOD", 0x7493},
	{"BL_ON", 0x7800},
	{"BL_OFF", 0x7801},
	{"BL_TOGG", 0x7802},
	{"BL_DEC", 0x7803},
	{"BL_INC", 0x7804},
	{"BL_STEP", 0x7805},
	{"BL_BRTG", 0x7806},
	{"RGB_TOG", 0x7820},
	{"RGB_MOD", 0x7821},
	{"RGB_RMOD", 0x7822},
	{"RGB_HUI", 0x7823},
	{"RGB_HUD", 0x7824},
	{"RGB_SAI", 0x7825},
	{"RGB_SAD", 0x7826},
	{"RGB_VAI", 0x7827},
	{"RGB_VAD", 0x7828},
	{"RGB_SPI", 0x7829},
	{"RGB_SPD", 0x782A},
	{"RGB_M_P", 0x782B},
	{"RGB_M_B", 0x782C},
	{"RGB_M_R", 0x782D},
	{"RGB_M_SW", 0x782E},
	{"RGB_M_SN", 0x782F},
	{"RGB_M_K", 0x7830},
	{"RGB_M_X", 0x7831},
	{"RGB_M_G", 0x7832},
	{"RGB_M_T", 0x7833},
	{"RESET", 0x7C00},
	{"ASDN", 0x7C10},
	{"ASUP", 0x7C11},
	{"ASRP", 0x7C12},
	{"ASON", 0x7C13},
	{"ASOFF", 0x7C14},
	{"ASTG", 0x7C15},
	{"GESC", 0x7C16},
	{"LCPO", 0x7C18},
	{"RCPC", 0x7C19},
	{"LSPO", 0x7C1A},
	{"RSPC", 0x7C1B},
	{"LAPO", 0x7C1C},
	{"RAPC", 0x7C1D},
	{"SFTENT", 0x7C1E},
	{"HPT_ON", 0x7C40},
	{"HPT_OFF", 0x7C41},
	{"HPT_TOG", 0x7C42},
	{"HPT_RST", 0x7C43},
	{"HPT_FBK", 0x7C44},
	{"HPT_BUZ", 0x7C45},
	{"HPT_MODI", 0x7C46},
	{"HPT_MODD", 0x7C47},
	{"HPT_CONT", 0x7C48},
	{"HPT_CONI", 0x7C49},
	{"HPT_COND", 0x7C4A},
	{"HPT_DWLI", 0x7C4B},
	{"HPT_DWLD", 0x7C4C},
	{"CMB_ON", 0x7C50},
	{"CMB_OFF", 0x7C51},
	{"CMB_TOG", 0x7C52},
	{"DYN_REC_START1", 0x7C53},
	{"DYN_REC_START2", 0x7C54},
	{"DYN_REC_STOP", 0x7C55},
	{"DYN_MACRO_PLY1", 0x7C56},
	{"DYN_MACRO_PLY2", 0x7C57},
	{"FN_MO13", 0x7C77},
	{"FN_MO23", 0x7C78}
};
int numElm4 = sizeof(kbName4) / sizeof(kbName4[0]);

int isInRange(uint16_t v, uint16_t start, uint16_t end) {
    return (v >= start && v <= end);
}

char* layerName(uint16_t keycode) {
  char* mod = ""; 
  switch (keycode) {
    case 0x5200:
      mod = "TO";
      break;
    case 0x5220:
      mod = "MO";
      break;
    case 0x5240:
      mod = "DF";
      break;
    case 0x5260:
      mod = "TG";
      break;
    case 0x5280:
      mod = "OSL";
      break;
    case 0x52C0:
      mod = "TT";
      break;
    case 0x5700:
      mod = "TD";
      break;
    case 0x7700:
      mod = "M";
      break;
    case 0x7E00:
      mod = "USER";
      break;
    default:
      break;
  }
  return mod;
}

char* modName(uint16_t keycode) {
  char* mod = ""; 
  switch (keycode) {
    case 0x1:
      mod = "LCTL";
      break;
    case 0x2:
      mod = "LSFT";
      break;
    case 0x3:
      mod = "C_S";
      break;
    case 0x4:
      mod = "LALT";
      break;
    case 0x5:
      mod = "LCA";
      break;
    case 0x6:
      mod = "LSA";
      break;
    case 0x7:
      mod = "MEH";
      break;
    case 0x8:
      mod = "LGUI";
      break;
    case 0x9:
      mod = "LCG";
      break;
    case 0xA:
      mod = "SGUI";
      break;
    case 0xD:
      mod = "LCAG";
      break;
    case 0xF:
      mod = "HYPR";
      break;
    case 0x11:
      mod = "RCTL";
      break;
    case 0x12:
      mod = "RSFT";
      break;
    case 0x14:
      mod = "RALT";
      break;
    case 0x18:
      mod = "RGUI";
      break;
    case 0x19:
      mod = "RCG";
      break;
    case 0x20:
      mod = "MT";
      break;
    case 0x21:
      mod = "LCTL_T";
      break;
    case 0x22:
      mod = "LSFT_T";
      break;
    case 0x23:
      mod = "C_S_T";
      break;
    case 0x24:
      mod = "LALT_T";
      break;
    case 0x25:
      mod = "LCA_T";
      break;
    case 0x26:
      mod = "LSA_T";
      break;
    case 0x27:
      mod = "MEH_T";
      break;
    case 0x28:
      mod = "LGUI_T";
      break;
    case 0x29:
      mod = "LCG_T";
      break;
    case 0x2A:
      mod = "SGUI_T";
      break;
    case 0x2D:
      mod = "LCAG_T";
      break;
    case 0x2F:
      mod = "ALL_T";
      break;
    case 0x31:
      mod = "RCTL_T";
      break;
    case 0x32:
      mod = "RSFT_T";
      break;
    case 0x34:
      mod = "RALT_T";
      break;
    case 0x38:
      mod = "RGUI_T";
      break;
    case 0x39:
      mod = "RCG_T";
      break;
    case 0x3D:
      mod = "RCAG_T";
      break;
    case 0x40:
      mod = "LT";
      break;
    case 0x50:
      mod = "LM";
      break;
    default:
      break;
  }
  return mod;
}

char* set_key_name(uint16_t keycode, struct Code2Name kbName[], int numElm) {
  char* key_name = "";
  for (int i = 0; i < numElm; i++) {
    char* key = kbName[i].key;
    uint16_t code = kbName[i].value;
    if(keycode == code){
      key_name = key;
      break;
    }
  }
  return key_name;
}

void set_code_to_name(uint16_t keycode) {
  strcpy(key_name, "UNDEF-KEY");

  if(isInRange(keycode, 0x00, 0xFF)){
    char* key = set_key_name(keycode, kbName1, numElm1);
    strcpy(key_name, key);
  }
  else if(isInRange(keycode, 0x21E, 0x238)){
    char* key = set_key_name(keycode, kbName2, numElm2);
    strcpy(key_name, key);
  }
  else if((isInRange(keycode, 0x4000, 0x4FFF) || isInRange(keycode, 0x5000, 0x51FF))){
    uint16_t firstTwoDigits = keycode & 0xFF;
    uint16_t thirdDigit = (keycode >> 8) & 0xF;
    uint16_t code = isInRange(keycode, 0x4000, 0x4FFF) ? 0x40 : 0x50;
    char* mod = modName(code);
    char* key = set_key_name(firstTwoDigits, kbName1, numElm1);
    snprintf(key_name, sizeof(key_name), "%s%d(%s)", mod, thirdDigit, key);
  }
  else if(
    isInRange(keycode, 0x5200, 0x57FF) ||
    isInRange(keycode, 0x7700, 0x77FF) ||
    isInRange(keycode, 0x7E00, 0x7EFF)
  ){
    uint16_t intDigit = (isInRange(keycode, 0x5200, 0x57FF) && !isInRange(keycode, 0x5700, 0x57FF)) ? keycode & 0xF : keycode & 0xFF;
    uint16_t twoThirdFourthDigits = isInRange(keycode, 0x5700, 0x57FF) ? keycode &= 0xFFF0 : keycode & 0xFFF0;
    char* mod = layerName(twoThirdFourthDigits);
    snprintf(key_name, sizeof(key_name), "%s(%d)", mod, intDigit);
  }
  else if(isInRange(keycode, 0x100, 0x3FFF)){
    uint16_t firstTwoDigits = keycode & 0xFF;
    uint16_t thirdFourthDigits = (keycode >> 8) & 0xFF;
    char* mod = modName(thirdFourthDigits);
    char* key = set_key_name(firstTwoDigits, kbName1, numElm1);
    snprintf(key_name, sizeof(key_name), "%s(%s)", mod, key);
  }
  else{
    char* key = set_key_name(keycode, kbName4, numElm4);
    strcpy(key_name, key);
  }
}