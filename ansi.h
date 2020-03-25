// ansi stuff, could always use printf instead of concat
String ansiPRE  = "\033"; // escape code
String ansiHOME = "\033[H"; // cursor home
String ansiESC  = "\033[2J"; // esc
String ansiCLC  = "\033[?25l"; // invisible cursor

String ansiEND  = "\033[0m";   // closing tag for styles
String ansiBOLD = "\033[1m";

String ansiRED  = "\033[41m"; // red background
String ansiGRN  = "\033[42m"; // green background
String ansiBLU  = "\033[44m"; // blue background

String ansiREDF = "\033[31m"; // red foreground
String ansiGRNF = "\033[34m"; // green foreground
String ansiBLUF = "\033[32m"; // blue foreground
String BELL     = "\a";

printf "\x1b[${bg};2;${red};${green};${blue}m\n"
printf "\x1b[38;2;255;100;0mTRUECOLOR\x1b[0m\n"

// Telnet.print(ansiHOME+ansiCLC); // clear screen
// Telnet.println("uptime: "+ansiBLU + ansiBOLD + (String)millis() + ansiEND + ansiEND+" ms");
