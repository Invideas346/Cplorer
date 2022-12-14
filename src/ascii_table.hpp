//
// Created by wolfgang on 07/08/2022.
//

#pragma once

typedef enum ascii_table
{
    // control characters
    _NULL, // NULL
    SOH,   // Start of Heading
    STX,   // Start of Text
    ETX,   // End of Text
    EOT,   // End of Transmission
    ENQ,   // Enquiry
    ACK,   // Acknowledgement
    BEL,   // Bell
    BS,    // Backspace
    TAB,   // Horizontal Tab
    LF,    // Line Feed
    VT,    // Vertical Tab
    FF,    // Form Feed
    CR,    // Carriage Return
    SO,    // Shift Out
    SI,    // Shift In
    DLE,   // Data Link Escape
    DC1,   // Device Control 1
    DC2,   // Device Control 2
    DC3,   // Device Control 3
    DC4,   // Device Control 4
    NAK,   // Negative Acknowledgement
    SYN,   // Synchronous Idle
    ETB,   // End of Transmission Block
    CAN,   // Cancel
    EM,    // End of Medium
    SUB,   // Substitute
    ESC,   // Escape
    SF,    // File Separator
    GS,    // Group Separator
    RS,    // Record Separator
    US,    // Unit Separator

    // misc characters

    SPACE,                 // space
    EXCLAMAION_MARK,       // !
    QUOTATION_MARK_DOUBLE, // "
    HASHTAG,               // #
    SING_DOLLAR,           // $
    PRECENT,               // %
    AMPERSANT,             // &
    QUOTATION_MARK_SINGLE, // '
    PARENTHESES_OPEN,      // (
    PARENTHESES_CLOSE,     // )
    ASTERISK,              // *
    PLUS,                  // +
    COMMA,                 // ,
    MINUS,                 // -
    DOT,                   // .
    SLASH_FOWARD,          // /
    NUMBER_0,              // 0
    NUMBER_1,              // 1
    NUMBER_2,              // 2
    NUMBER_3,              // 3
    NUMBER_4,              // 4
    NUMBER_5,              // 5
    NUMBER_6,              // 6
    NUMBER_7,              // 7
    NUMBER_8,              // 8
    NUMBER_9,              // 9
    COLON,                 // :
    COLON_SEMI,            // ;
    LESS_THAN,             // <
    EQUAL_TO,              // =
    GREATER_THAN,          // >
    QUESTION_MARK,         // ?
    SING_AT,               // @

    // upper case alphabet

    A_UPPER, // A
    B_UPPER, // B
    C_UPPER, // C
    D_UPPER, // D
    E_UPPER, // E
    F_UPPER, // F
    G_UPPER, // G
    H_UPPER, // H
    I_UPPER, // I
    J_UPPER, // J
    K_UPPER, // K
    L_UPPER, // L
    M_UPPER, // M
    N_UPPER, // N
    O_UPPER, // O
    P_UPPER, // P
    Q_UPPER, // Q
    R_UPPER, // R
    S_UPPER, // S
    T_UPPER, // T
    U_UPPER, // U
    V_UPPER, // V
    W_UPPER, // W
    X_UPPER, // X
    Y_UPPER, // Y
    Z_UPPER, // Z

    // misc characters

    BRACKET_OPEN,   // [
    SLASH_BACKWARD, //
    BRACKET_CLOSE,  // ]
    CARET,          // ^
    UNDERSCORE,     // _
    GRAVE_ACCENT,   // `

    // lower case alphabet

    A_LOWER, // a
    B_LOWER, // b
    C_LOWER, // c
    D_LOWER, // d
    E_LOWER, // e
    F_LOWER, // f
    G_LOWER, // g
    H_LOWER, // h
    I_LOWER, // i
    J_LOWER, // j
    K_LOWER, // k
    L_LOWER, // l
    M_LOWER, // m
    N_LOWER, // n
    O_LOWER, // o
    P_LOWER, // p
    Q_LOWER, // q
    R_LOWER, // r
    S_LOWER, // s
    T_LOWER, // t
    U_LOWER, // u
    V_LOWER, // v
    W_LOWER, // w
    X_LOWER, // x
    Y_LOWER, // y
    Z_LOWER, // z

    // misc characters

    BRACKET_CURLY_OPEN,  // {
    VERTICAL_BAR,        // |
    BRACKET_CURLY_CLOSE, // }
    TILDE,               // ~

    DEL, // Delete

    //===================[extended ASCII]===================//
    // misc letters

    C_UPPER_CEDILLA, // ??

    U_LOWER_UMLAUT,     // ??
    E_LOWER_ACUTE,      // ??
    A_LOWER_CIRCUMFLEX, // ??
    A_LOWER_UMLAUT,     // ??
    A_LOWER_GRAVE,      // ??
    A_LOWER_RING,       // ??
    C_LOWER_CEDILLA,    // ??
    E_LOWER_CIRCUMFLEX, // ??
    E_LOWER_UMLAUT,     // ??
    E_LOWER_GRAVE,      // ??
    I_LOWER_UMLAUT,     // ??
    I_LOWER_CIRCUMFLEX, // ??
    I_LOWER_GRAVE,      // ??
    A_UPPER_UMLAUT,     // ??
    A_UPPER_RING,       // ??
    E_UPPER_ACUTE,      // ??
    AE_LOWER,           // ??
    AE_UPPER,           // ??
    O_LOWER_CIRCUMFLEX, // ??
    O_LOWER_UMLAUT,     // ??
    O_LOWER_GRAVE,      // ??
    U_LOWER_CIRCUMFLEX, // ??
    U_LOWER_GRAVE,      // ??
    Y_LOWER_UMLAUT,     // ??
    O_UPPER_UMLAUT,     // ??
    U_UPPER_UMLAUT,     // ??

    SING_CENT,    // ??
    SING_POUND,   // ??
    SING_YEN,     // ??
    SING_PESTA,   // ???
    F_LOWER_HOOK, // ??

    A_LOWER_ACUTE, // ??
    I_LOWER_ACUTE, // ??
    O_LOWER_ACUTE, // ??
    U_LOWER_ACUTE, // ??
    N_LOWER_TILDE, // ??
    N_UPPER_TILDE, // ??

    // symbols

    ORDINAL_INDICATOR_FEMININE,        // ??
    ORDINAL_INDICATOR_MASCULINE,       // ??
    QUESTION_MARK_REVERSED,            // ??
    SING_NOT_REVERSED,                 // ???
    SING_NOT,                          // ??
    VULGAR_FRACTION_HALF,              // ??
    VULGAR_FRACTION_QUARTER,           // ??
    EXCLAMATION_MARK_INVERTED,         // ??
    QUOTATION_MARK_DOUBLE_ANGLE_LEFT,  // ??
    QUOTATION_MARK_DOUBLE_ANGLE_RIGHT, // ??

    /* box drawings
     * BOX_[direction1]_[thickness]_[direction2]_[thickness]
     * if you cant find a combination try switching the direction combination
     */

    SHADE_LIGHT,  // ???
    SHADE_MEDIUM, // ???
    SHADE_DARK,   // ???

    BOX_VERTICAL_LIGHT,                    // ???
    BOX_VERTICAL_LIGHT_LEFT_LIGHT,         // ???
    BOX_VERTICAL_SINGLE_LEFT_DOUBLE,       // ???
    BOX_VERTICAL_DOUBLE_LEFT_SINGLE,       // ???
    BOX_DOWN_DOUBLE_LEFT_SINGLE,           // ???
    BOX_DOWN_SINGLE_LEFT_DOUBLE,           // ???
    BOX_VERTICAL_DOUBLE_LEFT_DOUBLE,       // ???
    BOX_VERTICAL_DOUBLE,                   // ???
    BOX_DOWN_DOUBLE_LEFT_DOUBLE,           // ???
    BOX_UP_DOUBLE_LEFT_DOUBLE,             // ???
    BOX_UP_DOUBLE_LEFT_SINGLE,             // ???
    BOX_UP_SINGLE_LEFT_DOUBLE,             // ???
    BOX_DOWN_LIGHT_LEFT_LIGHT,             // ???
    BOX_UP_LIGHT_RIGHT_LIGHT,              // ???
    BOX_UP_LIGHT_HORIZONTAL_LIGHT,         // ???
    BOX_DOWN_LIGHT_HORIZONTAL_LIGHT,       // ???
    BOX_VERTICAL_LIGHT_RIGHT_LIGHT,        // ???
    BOX_HORIZONTAL_LIGHT,                  // ???
    BOX_VERTICAL_LIGHT_HORIZONTAL_LIGHT,   // ???
    BOX_VERTICAL_SINGLE_RIGHT_DOUBLE,      // ???
    BOX_VERTICAL_DOUBLE_RIGHT_SINGLE,      // ???
    BOX_UP_DOUBLE_RIGHT_DOUBLE,            // ???
    BOX_DOWN_DOUBLE_RIGHT_DOUBLE,          // ???
    BOX_UP_DOUBLE_HORIZONTAL_DOUBLE,       // ???
    BOX_DOWN_DOUBLE_HORIZONTAL_DOUBLE,     // ???
    BOX_VERTICAL_DOUBLE_RIGHT_DOUBLE,      // ???
    BOX_HORIZONTAL_DOUBLE,                 // ???
    BOX_VERTICAL_DOUBLE_HORIZONTAL_DOUBLE, // ???
    BOX_UP_SINGLE_HORIZONTAL_DOUBLE,       // ???
    BOX_UP_DOUBLE_HORIZONTAL_SINGLE,       // ???
    BOX_DOWN_SINGLE_HORIZONTAL_DOUBLE,     // ???
    BOX_DOWN_DOUBLE_HORIZONTAL_SINGLE,     // ???
    BOX_UP_DOUBLE_RIGHT_SINGLE,            // ???
    BOX_UP_SINGLE_RIGHT_SINGLE,            // ???
    BOX_DOWN_SINGLE_RIGHT_DOUBLE,          // ???
    BOX_DOWN_SINGLE_RIGHT_SINGLE,          // ???
    BOX_VETRICAL_DOUBLE_HORIZONTAL_SINGLE, // ???
    BOX_VERTICAL_SINGLE_HORIZONTALDOUBLE,  // ???
    BOX_UP_LIGHT_LEFT_LIGHT,               // ???
    BOX_DOWN_LIGHT_RIGHT_LIGHT,            // ???

    BLOCK_FULL,       // ???
    BLOCK_HALF_LOWER, // ???
    BLOCK_HALF_LEFT,  // ???
    BLOCK_HALF_RIGHT, // ???
    BLOCK_HALF_UPPER, // ???

    // other letters

    APLHA_LOWER,   // ??
    S_SHARP_LOWER, // ??
    GAMMA_UPPER,   // ??
    PI_LOWER,      // ??
    SIGMA_UPPER,   // ??
    SIGMA_LOWER,   // ??

    SING_MICRO,               // ??
    TAU_LOWER,                // ??
    PHI_UPPER,                // ??
    THETA_UPPER,              // ??
    OMEGA_UPPER,              // ??
    DELTA_LOWER,              // ??
    INFINITY,                 // ???
    PHI_LOWER,                // ??
    EPSILON_LOWER,            // ??
    INTERSECTION,             // ???
    IDENTICAL_TO,             // ???
    SING_PLUS_MINUS,          // ??
    GREATER_THAN_OR_EQUAL_TO, // ???
    LESS_THAN_OR_EQUAL_TO,    // ???
    HALF_INTEGRAL_TOP,        // ???
    HALF_INTEGRAL_BOTTOM,     // ???
    SING_DIVISION,            // ??
    EQUAL_TO_ALMOST,          // ???
    SING_DEGREE,              // ??
    BULLER_OPERATOR,          // ???
    DOT_MIDDLE,               // ??
    SQUARE_ROOT,              // ???
    N_SUPERSCRIPT_LOWER,      // ???
    NUMBER_2_SUPERSCRIPT,     // ??
    BLACK_SQUARE,             // ???
    SPACE_NO_BREAK,           //
} ascii_table;