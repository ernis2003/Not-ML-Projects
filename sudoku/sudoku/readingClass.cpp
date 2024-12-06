#include"libraries.h";
using namespace std;

class Reading{
    private:
        int rows_Q;
        int cols_Q;
        char separator_;
        string fileName_;
        vector<vector<int>> matrix;
    public:
        Reading(string fileName, char separator){
            fileName_ = fileName;
            separator_ = separator;
            rows_Q = 0;
            cols_Q = 0;
        }
    
        vector<char> ReadFile(){
            vector<char> sudokuData = {};

            fstream FileReader(fileName_);
            string line;
            int rowCounter = 0;
            while(getline(FileReader, line)){
                int colCounter = 0;
                if(!line.empty() && line.at(0) != '/' && line.at(0) != '#') {
                    for(int i = 0; i < line.length(); i++){
                        if((line.at(i) != ' ') == 1){
                            sudokuData.push_back(line.at(i));
                            colCounter++;
                        }
                    }
                    if(cols_Q < colCounter){
                        cols_Q = colCounter;
                    }
                    rowCounter++;
                }
            }
            rows_Q = rowCounter;
            if(rows_Q != cols_Q){
                cout << "Invalid size of square:\n" << "rows quantity: " << rows_Q
                << "\ncolumn quantity: " << cols_Q << endl;
                return vector<char> {' ', ' '};
            }
            cout << "Total rows: " << rows_Q <<
            "\nTotal cols: " << cols_Q << endl;
            return sudokuData;
        }

        vector<vector<int>> FormMatrix(){
            vector<vector<int>> finalMatrix = {};
            vector<char> singleArr = ReadFile();
            if(singleArr.size() > 2){
                vector<int> data = {};
                for(int i = 0; i < singleArr.size(); i++){
                    if(remainder(i, rows_Q) == 0 && i != 0){
                        finalMatrix.push_back(data);
                        data = {};
                    }
                    if(singleArr.at(i) == '-'){
                        data.push_back(-1);
                    }
                    else{
                        data.push_back(singleArr.at(i) - '0');
                    }
                }
                finalMatrix.push_back(data);
            }
            matrix = finalMatrix;
            return finalMatrix;
        }
        vector<vector<int>> GetMatrix(){
            return FormMatrix();
        }

        vector<int> GetParams(){
            return {rows_Q, cols_Q};
        }
};