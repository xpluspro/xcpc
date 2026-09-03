dfa[0][P[0] - 'a'] = 1;
int X = 0; 
for (int i = 1; i < m; i++) {
    for (int c = 0; c < 26; c++) dfa[i][c] = dfa[X][c]; 
    dfa[i][P[i] - 'a'] = i + 1;
    X = dfa[X][P[i] - 'a']; }