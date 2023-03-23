#include "applogic.h"
using namespace std;

FuncReturningValue* getDataFromFile(const char *filename, char* region);
FuncReturningValue* solve(char ***data, size_t len, size_t field);
void clean(FuncArgument* args);
void clean(FuncReturningValue* frv);
char** strSplit(char*, size_t *, const char);
char** readfile(ifstream&, size_t*, char*);
void clean2DArray(char**, size_t );
void clean3DArray(char***, size_t, size_t);
double sort(char***, double*, size_t, size_t);
double charToDouble(double*, char***, size_t, size_t);
double* charToDoublePlot(double*, char***, size_t, size_t);
void fillFrvSolution(FuncReturningValue*, double*, size_t);
void isWords(char* str, bool* arr, int len);
void fillArrByTok(char**, char*, char*);
FuncReturningValue* fillPltValues(FuncReturningValue* frv, double* arr, size_t newSize, size_t len, size_t field, char*** data);
template <typename T>
bool isNULL(T);


FuncReturningValue* entryPoint(FuncType ft, FuncArgument* fa)
{
    FuncReturningValue* result;
    switch(ft)
    {
        case getData:
            result = getDataFromFile(fa->filename, fa->region);
            break;
        case calculateData:
            result = solve(fa->data, fa->len, fa->field);
            break;
        case cleanData:
            clean(fa);
        default:
            result = NULL;
            break;
    }
    return result;
}

FuncReturningValue* getDataFromFile(const char* filename, char* region)
{
    FuncReturningValue *frv = (FuncReturningValue *)malloc(sizeof(FuncReturningValue));
    if(isNULL(frv))
        return NULL;
    ifstream file;
    file.open(filename);
    if(file.is_open()){
        size_t lines, fields;
        char **rawData = readfile(file, &lines, region);
        if(rawData != NULL){
            char ***data = (char***)malloc((lines - 1) * sizeof(char**));
            if(data != NULL){
                for (size_t i = 0; i < lines - 1; i++){
                    data[i] = strSplit(rawData[i+1], &fields, ',');
                }
                char **headers = strSplit(rawData[0], &fields, ',');
                lines--;
                clean2DArray(rawData, lines);
                frv->len = lines;
                frv->region = region;
                frv->fields_num = fields;
                frv->headers = headers;
                frv->data = data;
            }else{
                clean2DArray(rawData, lines);
                clean3DArray(data, lines, lines-1);
            }
        }
    }
    file.close();
    return frv;
}

FuncReturningValue* solve(char*** data, size_t len, size_t field)
{
    FuncReturningValue *frv = (FuncReturningValue *)malloc(sizeof(FuncReturningValue));
    if(isNULL(frv))
        return 0;
    double* arr = (double*)malloc(sizeof(double) * len);
    if(arr != NULL){
        frv->fields_num = 3;
        size_t newSize = sort(data, arr, len, field);
        arr = (double*)realloc(arr, sizeof(double) * newSize);
        if(arr != NULL){
            fillFrvSolution(frv, arr, newSize);
            frv = fillPltValues(frv, arr, newSize, len, field, data);
        }
    }
    free(arr);
    return frv;
}

FuncReturningValue* fillPltValues(FuncReturningValue* frv, double* arr, size_t newSize, size_t len, size_t field, char*** data)
{
    frv->valuesX = (double*)malloc(sizeof(double)* len);
    frv->plotValues = (double*)malloc(sizeof(double)* len);
    frv->valuesY = (double*)malloc(sizeof(double)*VALUES_Y_SIZE);
    if(arr != NULL and frv->valuesX != NULL and frv->plotValues != NULL and frv->valuesY != NULL){
        double step = (arr[newSize-1] - arr[0])/10;
        frv->valuesY[0] = arr[0];
        frv->valuesY[VALUES_Y_SIZE-1] = arr[newSize-1];
        for(int i = 1; i < VALUES_Y_SIZE-1; i++){
            frv->valuesY[i] = arr[0]+i*step;
        }
        frv->valuesXSize = charToDouble(frv->valuesX, data, len, 0);
        frv->plotValues = charToDoublePlot(frv->plotValues, data, len, field);
        frv->plotValuesSize = len;
    }
    return frv;
}


void fillFrvSolution(FuncReturningValue* frv, double* arr, size_t newSize){
    if(newSize == EMPTY){
        frv->solution[MAX_VALUE_INDEX] = 0;
        frv->solution[MIN_VALUE_INDEX] = 0;
        frv->solution[MID_VALUE_INDEX] = 0;
    }else{
        frv->solution[MAX_VALUE_INDEX] = arr[newSize-1];
        frv->solution[MIN_VALUE_INDEX] = arr[0];
            if(newSize % 2 != 0){
                frv->solution[MID_VALUE_INDEX] = arr[(newSize-1)/2];
            }else{
                double tmp1, tmp2;
                tmp1 = arr[((newSize-1)/2)];
                tmp2 = arr[((newSize-1)/2 + 1)];
                frv->solution[MID_VALUE_INDEX] = (tmp1+tmp2)/2;
            }
    }
}

double* charToDoublePlot(double* arr, char***data, size_t len, size_t field)
{
    for(size_t i = 0; i < len; i++){
            string str = data[i][field];
            if(str.empty()){
                arr[i] = EMPTY;
            }else{
                arr[i] = atof(((data[i][field])));
            }
        }
    return arr;
}

double charToDouble(double* arr, char***data, size_t len, size_t field)
{
    double size = len;
    int cnt = 0;
    for(size_t i = 0; i < len; i++){
            string str = data[i][field];
            if(!str.empty()){
                arr[cnt++] = atof(((data[i][field])));
            }else{
                size--;
            }
        }
    return size;
}

double sort(char*** data, double* num, size_t len, size_t field)
{
    double tmp;
    double size = charToDouble(num, data, len, field);
    if(size != 0){
        for (size_t i = 0; i < size-1; i++){
              for (size_t j = 0; j < size-i-1; j++){
                  if (num[j] > num[j+1]){
                      tmp = num[j];
                      num[j] = num[j+1];
                      num[j+1] = tmp;
                  }
              }
        }
    }else
        size = EMPTY;
    return size;
}

char** strSplit(char* a_str, size_t *len, const char a_delim)
{
    char** result = 0;
    size_t count = 0;
    char* tmp = a_str;
    char delim[2];
    delim[0] = a_delim;
    while (*tmp){
        if (a_delim == *tmp){
            count++;
        }
        tmp++;
    }
    count++;
    *len = count;
    result = (char**)malloc(sizeof(char*) * count);
    if(result != NULL){
        fillArrByTok(result, a_str, delim);
    }else
        clean2DArray(result, *len);
    return result;
}

void fillArrByTok(char** arr, char* a_str, char* delim)
{
    char* p = a_str;
    char* token;
    int idx = 0;
    token = strsep(&p, delim);
    while(token){
        arr[idx] = strdup(token);
        token = strsep(&p, delim);
        idx++;
    }
}

bool isYear(string str, double* year){
    bool res = false;
    double from = year[YEAR_FROM_INDEX];
    double to = year[YEAR_TO_INDEX];
    if(year[YEAR_FROM_INDEX] != EMPTY && year[YEAR_TO_INDEX] == EMPTY)
            res = true;
    else if(year[YEAR_FROM_INDEX] != EMPTY && year[YEAR_TO_INDEX] == EMPTY){
        size_t i = str.find(from);
        if(i != string::npos)
            res = true;
    }

    else if(year[YEAR_FROM_INDEX] == EMPTY && year[YEAR_TO_INDEX] != EMPTY){
        size_t i = str.find(to);
//        char* year = strncpy(year, str.c_str(), i);
        if(i != string::npos)
            res = true;
//        else if(strcmp(str.c_str(), year) > 0)
//            res = true;
    }
    return res;
}

char** readfile(ifstream &file, size_t *lines, char* region)
{
    size_t llen, counter = 0, max_size = 1;
    string str;
    char **data = (char **)calloc(max_size, sizeof(char *));
    if(data != NULL){
        while(!file.eof()){
            getline(file, str);
            const char *str_tmp = str.c_str();
            if(str_tmp != NULL){
                if((strstr(str_tmp, region))){
                    if (counter >= max_size-1){
                        data = (char **)realloc(data,max_size * 2 * sizeof(char *));
                        if(data != NULL)
                            max_size *= 2;
                    }
                    if(data != NULL){
                        llen = strlen(str.c_str());
                        data[counter] = (char *)calloc(sizeof(char), llen+1);
                        if(data[counter] != NULL){
                            strcpy(data[counter++], str_tmp);
                        }
                    }else
                        free(data);
                }
            }
        }
        *lines = counter;
        str.clear();
    }
    return data;
}

void clean2DArray(char **arr, size_t size)
{
    for (size_t i = 0; i < size; i++){
        free(arr[i]);
    }
    free(arr);
}


void clean(FuncArgument* args)
{
    if (args->data != NULL){
        clean3DArray(args->data, args->len, args->fields_num);
    }
    if (args->filename != NULL){
        free(args->filename);
    }
    if (args->headers != NULL){
        clean2DArray(args->headers, args->fields_num);
    }
    if(args->region != NULL){
        free(args->region);
    }
}

void clean3DArray(char ***arr, size_t sizeX, size_t sizeY)
{
    for (size_t i = 0; i < sizeX; i++){
        clean2DArray(arr[i], sizeY);
    }
    free(arr);
}

template <typename T>
bool isNULL(T t){
    if(t == NULL){
        free(t);
        return(true);
    }else
        return(false);
}

