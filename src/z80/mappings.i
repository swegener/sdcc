static const ASM_MAPPING _asxxxx_gb_mapping[] = {
    { "*hl", "(hl)" },
    { "di", "di" },
    { "ldahli", "ld a,(hl+)" },
    { "ldahlsp", "lda hl,%d(sp)" },
    { "ldaspsp", "lda sp,%d(sp)" },
    { "*pair", "(%s)" },
    { "shortjp", "jr" },
    { "enter", "push bc" },
    { "enterx", 
      "push bc\n"
      "lda sp,-%d(sp)" },
    { "leave", 
      "pop bc\n"
      "\tret"
    },
    { "leavex", 
      "lda sp,%d(sp)\n"
      "\tpop bc\n"
      "\tret"
    },
    { "pusha", 
      "push af\n"
      "\tpush bc\n"
      "\tpush de\n"
      "\tpush hl"
    },
    { "adjustsp", "lda sp,-%d(sp)" },
    { NULL, NULL }
};

static const ASM_MAPPING _asxxxx_z80_mapping[] = {
    { "*hl", "(hl)" },
    { "di", "di" },
    { "ldahli", "ld a,(hl+)" },
    { "ldahlsp", "lda hl,%d(sp)" },
    { "ldaspsp", "lda sp,%d(sp)" },
    { "*pair", "(%s)" },
    { "shortjp", "jr" },
    { "enter", "push bc" },
    { "enterx", 
      "push bc\n"
      "lda sp,-%d(sp)" },
    { "leave", 
      "pop bc\n"
      "\tret"
    },
    { "leavex", 
      "lda sp,%d(sp)\n"
      "\tpop bc\n"
      "\tret"
    },
    { "pusha", 
      "push af\n"
      "\tpush bc\n"
      "\tpush de\n"
      "\tpush hl"
    },
    { "adjustsp", "lda sp,-%d(sp)" },
    { NULL, NULL }
};

static const ASM_MAPPING _rgbds_mapping[] = {
    { "global", "GLOBAL %s" },
    { "labeldef", "%s:" },
    { "tlabeldef", ".l%05d:" },
    { "tlabel", ".l%05d" },
    { "fileprelude", 
      "; Generated using the rgbds tokens.\n"
      "\t; We have to define these here as sdcc dosnt make them global by default\n"
      "\tGLOBAL __mulschar\n"
      "\tGLOBAL __muluchar\n"
      "\tGLOBAL __mulsint\n"
      "\tGLOBAL __muluint\n"
      "\tGLOBAL __divschar\n"
      "\tGLOBAL __divuchar\n"
      "\tGLOBAL __divsint\n"
      "\tGLOBAL __divuint\n"
      "\tGLOBAL __modschar\n"
      "\tGLOBAL __moduchar\n"
      "\tGLOBAL __modsint\n"
      "\tGLOBAL __moduint"
    },
    { "functionheader", 
      "; ---------------------------------\n"
      "; Function %s\n"
      "; ---------------------------------"
    },
    { "functionlabeldef", "%s:" },
    { "zero", "$00" },
    { "one", "$01" },
    { "area", "SECTION \"%s\",CODE" },
    { "areacode", "SECTION \"CODE\",CODE" },
    { "areadata", "SECTION \"DATA\",BSS" },
    { "ascii", "DB \"%s\"" },
    { "ds", "DS %d" },
    { "db", "DB %d" },
    { "dbs", "DB %s" },
    { "dw", "DW %d" },
    { "dws", "DW %s" },
    { "immed", "" },
    { "constbyte", "$%02X" },
    { "constword", "$%04X" },
    { "immedword", "$%04X" },
    { "immedbyte", "$%02X" },
    { "hashedstr", "%s" },
    { "lsbimmeds", "%s & $FF" },
    { "msbimmeds", "%s >> 8" },
    { "module", "; MODULE %s" },
    { NULL, NULL }
};

static const ASM_MAPPING _rgbds_gb_mapping[] = {
    { "pusha", 
      "push af\n"
      "\tpush bc\n"
      "\tpush de\n"
      "\tpush hl"
    },
    { "di", "di" },
    { "adjustsp", "add sp,-%d" },
    { "enter", "push bc" },
    { "enterx", 
      "push bc\n"
      "\tadd sp,-%d"
    },
    { "leave", 
      "pop bc\n"
      "\tret"
    },
    { "leavex", 
      "add sp,%d\n"
      "\tpop bc\n"
      "\tret"
    },
    { "ldahli", "ld a,[hl+]" },
    { "*hl", "[hl]" },
    { "ldahlsp", "ld hl,[sp+%d]" },
    { "ldaspsp", "add sp,%d" },
    { "*pair", "[%s]" },
    { "shortjp", "jr" },
    { NULL, NULL }
};

static const ASM_MAPPINGS _rgbds = {
    NULL,
    _rgbds_mapping
};

const ASM_MAPPINGS _rgbds_gb = {
    &_rgbds,
    _rgbds_gb_mapping
};

const ASM_MAPPINGS _asxxxx_gb = {
    &asm_asxxxx_mapping,
    _asxxxx_gb_mapping
};

const ASM_MAPPINGS _asxxxx_z80 = {
    &asm_asxxxx_mapping,
    _asxxxx_z80_mapping
};
