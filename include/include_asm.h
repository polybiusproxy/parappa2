#ifndef INCLUDE_ASM_H
#define INCLUDE_ASM_H

#if !defined(SPLAT) && !defined(M2CTX) && !defined(PERMUTER)
#ifndef INCLUDE_ASM
#define INCLUDE_ASM(FOLDER, NAME)                                       \
    __asm__(                                                            \
        ".section .text\n"                                              \
        "   .set noat\n"                                                \
        "   .set noreorder\n"                                           \
        "   .globl\t" #NAME ".NON_MATCHING\n"                           \
        "   .ent\t" #NAME "\n" #NAME ".NON_MATCHING:\n"                 \
        "   .include \"asm/nonmatchings/" FOLDER "/" #NAME ".s\"\n"     \
        "   .set reorder\n"                                             \
        "   .set at\n"                                                  \
        "   .end\t" #NAME                                               \
    );
#endif
#ifndef INCLUDE_RODATA
#define INCLUDE_RODATA(FOLDER, NAME)                                    \
    __asm__(                                                            \
        ".section .rodata\n"                                            \
        "    .include \"asm/nonmatchings/" FOLDER "/" #NAME ".s\"\n"    \
        ".section .text"                                                \
    )
#endif
__asm__(".include \"include/macro.inc\"\n");
#else
#ifndef INCLUDE_ASM
#define INCLUDE_ASM(FOLDER, NAME)
#endif
#ifndef INCLUDE_RODATA
#define INCLUDE_RODATA(FOLDER, NAME)
#endif
#endif

#endif /* INCLUDE_ASM_H */