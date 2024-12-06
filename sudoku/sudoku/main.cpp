#include"libraries.h";
#include"readingClass.cpp"
#include"sudokuClass.cpp"
using namespace std;

int main(){
    Reading reading("sudoku.txt", ' ');
    vector<vector<int>> matrix = reading.GetMatrix();
    vector<int> matrixParams = reading.GetParams();


    if(matrixParams[0] == matrixParams[1]){
        const int singleLineLength = round(matrixParams[0] * 7.66);
        Sudoku sudoku(matrixParams[0], matrixParams[1], matrix, singleLineLength);
        vector<string> wasModified = sudoku.validateData();
        sudoku.PrintMatrix("MATRIX PRINTING", false, -1, -1);

        if (wasModified[0] == "1" ? true : false) {
            cout << "\n\nAt least " << stoi(wasModified[1]) / 2 << " incorrect data was detected.It will be replaced!\n" << endl; // konvertuoti i char ir dalinti is 2
        }
        bool solved = sudoku.solveSudoku();
        if (!solved) {
            cout << "\nSudoku can't be solved. Check sudoku correctness!\n" << endl;
        }
        else {
            sudoku.PrintMatrix("SOLVED MATRIX", true, 13, 0);
        }
    }
    else {
        cout << "\n\nCheck dimensions of sudoku!\n\n" << endl;
    }
    return 0;
}
