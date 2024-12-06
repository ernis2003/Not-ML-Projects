#include"libraries.h";
#include"helpFunctions.cpp";

using namespace std;

class Sudoku{
    private:
        int rows_Q;
        int cols_Q;
        vector<vector<int>> solvedSudoku;
        vector<vector<int>> originalSudoku;
        map<int, vector<int>> gretimumoMatrix;
        vector<vector<int>> incorrectData;
        vector<int> visitedNodes;
        string line;
    public:
        Sudoku(int rowsQ, int colsQ, vector<vector<int>> matrix, int lineLength){
            rows_Q = rowsQ;
            cols_Q = colsQ;
            solvedSudoku = matrix;
            originalSudoku = matrix;
            line = duplicate("-", lineLength);
        }
        /*
        Getting an gretimumo matrix.
        Example:
            0: 1, 3, 2, 4
            1: 0, 2, 4
            2: 0, 1, 3
            3: 0, 2
            4: 0
        Gretimumo matrix show us how graph nodes are connected
        */
         void EdgeMatrix(){
             map<int, vector<int>> data = {};
             int totalIterator = 0;
             for(int i = 0; i < rows_Q; i++){
                 for(int j = 0; j < cols_Q; j++){
                     vector<int> cellConnections = {};
                     //Putting all row values

                     for(int k = 0; k < 2; k++){
                         if(k == 1){
                             for(int m = 0; m < cols_Q; m++){ //pridedami stulpelio elementai
                                 if(i != m){
                                     cellConnections.push_back(1 + j + m * rows_Q); 
                                 }
                             }
                             break;
                         }
                         else{ //pridedami eilutes elementai
                             for(int m = 0; m < rows_Q; m++){
                                 if(j != m){
                                     cellConnections.push_back(m + i * cols_Q  + 1);
                                 }
                             }
                         }
                     }
                     data.emplace(totalIterator++, cellConnections);
                 }
             }
             gretimumoMatrix = data;
         }
         /*
            Returning true if data were incorrect, otherwise - false;
         */
         vector<string> validateData() {
             bool changed = false;
             int amount = 0;
             for (int i = 0; i < rows_Q; i++) {
                 for (int j = 0; j < cols_Q; j++) {
                     if (!isValidValue(i, j, originalSudoku[i][j], true) && originalSudoku[i][j] != -1) {
                         incorrectData.push_back({i, j}); //sulyginti kai printinu matrica ir pazymeti raudonai
                         solvedSudoku[i][j] = -1;
                         changed = true;
                         amount++;
                     }
                 }
             }
             return vector<string> {to_string(changed), to_string(amount)};
         }
         void updateIncorrectValues() {
             for (int i = 0; i < incorrectData.size(); i++) {
                 originalSudoku[incorrectData[i][0]][incorrectData[i][1]] = -1;
             }
         }
        void PrintMatrix(string text, bool printOriginal, int textColor, int bgColor){
            cout << text << endl;
            for(int i = 0; i < rows_Q; i++){
                cout << "\t" + line << endl;
                SetColor(7, 0);
                
                for(int j = 0; j < cols_Q; j++){
                    if(!printOriginal){
                        if (ContainsValue(i, j)) { //if incorrect input data
                            SetColor(15, 1);
                            cout << "\t| " << originalSudoku[i][j] << " |";
                        }
                        else {
                            cout << "\t| " + ((originalSudoku[i][j] == -1) ? "-" : to_string(originalSudoku[i][j])) + " |";

                        }
                    }
                    else{
                        if (originalSudoku[i][j] == -1) {
                            SetColor(textColor, bgColor);
                            cout << "\t| " << solvedSudoku[i][j] << " |";
                        }
                        else {
                            cout << "\t| " << solvedSudoku[i][j] << " |";
                        }
                    }
                    SetColor(7, 0);
                }   
                cout << endl;
            }
            cout << "\t" + line << endl;
        }
        bool ContainsValue(int iIndex, int jIndex) {
            for (int i = 0; i < incorrectData.size(); i++) {
                if (incorrectData[i][0] == iIndex && incorrectData[i][1] == jIndex) {
                    return true;
                }
            }
            return false;
        }

        void PrintEdgeMatrix(string text){ //"EDGE MATRIX PRINTING"
            EdgeMatrix();
            cout << text << endl;
            for(int i = 0; i < rows_Q*cols_Q; i++){
                vector<int> values = gretimumoMatrix[i];
                cout << i+1 << ":";
                    for(int j = 0; j < (rows_Q-1)*2; j++){
                        cout << "\t" << values[j];
                    }
                cout << endl;
            }
        }

        bool solvingSudoku() {
            for(int iCoord = 0; iCoord < rows_Q; iCoord++){
                for(int jCoord = 0; jCoord < cols_Q; jCoord++){

                    if(solvedSudoku[iCoord][jCoord] == -1){
                        for(int number = 1; number < 10; number++){
                            if(isValidValue(iCoord, jCoord, number, false)){

                                solvedSudoku[iCoord][jCoord] = number;
                                if(solvingSudoku()){
                                    return true;
                                };
                                solvedSudoku[iCoord][jCoord] = -1;
                            }
                        }
                        return false;
                    }

                }   
            }
            return true;
        }

        bool solveSudoku() {
            updateIncorrectValues();
            bool solvedSudoku = solvingSudoku();
            return solvedSudoku;
        }

        bool isValidValue(int iCoord, int jCoord, int value, bool validateInput){
            bool isRowValid =  checkLine(true, iCoord, jCoord, value, validateInput);
            bool isColValid = checkLine(false, iCoord, jCoord, value, validateInput);
            bool isValidSquare = true;

            if(rows_Q % 2 == 0 && iCoord + jCoord != 0){ //Jeigu lyginis kiekis
                isValidSquare = isValidBlock(2, iCoord, jCoord, value, validateInput);
            }
            else if(iCoord + jCoord != 0){
                isValidSquare = isValidBlock(3, iCoord, jCoord, value, validateInput);
            }
            return isRowValid && isColValid && isValidSquare;
        }

        bool isValidBlock(int blockSize, int iCoord, int jCoord, int value, bool checkOriginal) {
            int blockRowStart = iCoord / blockSize * blockSize;
            int blockColStart = jCoord / blockSize * blockSize;

            if (!checkOriginal) {
                for (int i = blockRowStart; i < blockRowStart + blockSize; i++) { //Check box
                    for (int j = blockColStart; j < blockColStart + blockSize; j++) {
                        if (solvedSudoku[i][j] == value) { return false; }
                    }
                }
            }
            else {
                for (int i = blockRowStart; i < blockRowStart + blockSize; i++) { //Check box
                    for (int j = blockColStart; j < blockColStart + blockSize; j++) {
                        if (originalSudoku[i][j] == value && originalSudoku[iCoord][jCoord] != value) { return false; }
                    }
                }
            }
            return true;
        }

        bool checkLine(bool checkRow, int iCoord, int jCoord, int value, bool checkOriginal) {
            if (!checkOriginal) {
                if (checkRow) {
                    for (auto rowValue : solvedSudoku[iCoord]) { //Check row
                        if (rowValue == value) {
                            return false;
                        }
                    }
                }
                else {
                    for (auto vectors : solvedSudoku) { //Check column
                        if (value == vectors[jCoord]) {
                            return false;
                        }
                    }
                }
            }
            else {
                if (checkRow) {
                    for (int j = 0; j < originalSudoku[iCoord].size(); j++) { //Check row
                        if (originalSudoku[iCoord][j] == value && jCoord != j) { //Tikrinti ar eiluteje yra elementai
                            return false;
                        }
                    }
                }
                else {
                    for (auto vectors : originalSudoku) { //Check column
                        if (value == vectors[jCoord] && originalSudoku[iCoord][jCoord] != value) {
                            return false;
                        }
                    }
                }
            }
            return true;
        }
        string duplicate(string value, int quantity) {
            string finalString = "";
            for (int i = 0; i < quantity; i++) {
                finalString += value;
            }
            return finalString;
        }

        void SetColor(int textColor, int bgColor) {
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole, (bgColor << 4) | textColor);
        }
};
