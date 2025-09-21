// void print_bitboard(PieceBB pieces[])
// {
//     for (int rank = 7; rank >= 0; rank--)
//     {
//         printf("%d ", rank + 1);
//         for (int file = 0; file < 8; file++)
//         {
//             int sq = rank * 8 + file;
//             uint64_t mask = (uint64_t)1 << sq;
//             char piece = '.';

//             for (int i = 0; i < 12; i++)
//             {
//                 if (*(pieces[i].bb) & mask)
//                 {
//                     piece = pieces[i].symbol;
//                 }
//             }
//             printf("%c", piece);
//         }
//         printf("\n");
//     }
//     printf("  abcdefgh\n");
// }

// Square get_input_square(void)
// {
//     char input[100]; // big enough to hold the whole line

//     while (1)
//     {
//         printf("Enter square (i.e. h1): ");

//         if (!fgets(input, sizeof(input), stdin))
//         {
//             // Input error or EOF
//             Square invalid = {-1, -1};
//             return invalid;
//         }

//         // Remove trailing newline if it exists
//         input[strcspn(input, "\n")] = '\0';

//         // Check length (must be exactly 2: e.g. 'h' and '1')
//         if (strlen(input) != 2 || input[0] > 'h' || input[1] > '8')
//         {
//             printf("Invalid input, please enter like h1.\n");
//             continue; // ask again
//         }

//         char file_char = input[0];
//         char row_char = input[1];

//         int file = file_char - FILE_OFFSET;
//         int row = row_char - ROW_OFFSET;

//         Square square = {file, row};
//         return square;
//     }
// }