#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <array>
#include <string>
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <bits/stdc++.h> 

using namespace std; 
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::array;
using std::filesystem::directory_iterator;
namespace az = std::filesystem;

bool m_cmt = false;

// d is the number of characters in the input alphabet
#define d 256

class FileChecking 
{
protected:
    int numFiles;
    string path;
    string filename;
    fstream myfile;

public:
    FileChecking()
    {
        numFiles = 0;
    }
    
    //Display all files with extension from full directory path
    void readFileName(string path)
    {
        for (const auto & file : directory_iterator(path))
        {
            cout << "File name with full directory path: " << endl;
            cout << file.path() << endl; //full directory path

            cout << "File name: " << endl;
            cout << az::path(file.path()).filename() << endl; //only files name
            cout << endl;
        }
    }
    
    //extract all file names in a folder and put them in a file (StudentsID.txt)  
    void extractFileName(string path)
    {
        ofstream outfile1("File.txt");    //create File.txt to store file names
        fstream fout1;
        fout1.open("File.txt");

        for (const auto &file : directory_iterator(path))
            fout1 << az::path(file.path()).filename() << endl;
        fout1.close();

        ofstream outfile2("StudentsID.txt");    //create StudentsID.tct to store all student ID
        fstream fout2, fin2;
        fin2.open("File.txt");
        fout2.open("StudentsID.txt");
        string line; 

        char *array[numFiles];  //to hold all student ID 

        for(int i = 0; i < numFiles; i++)
            array[i] = (char*)malloc(21);  // allocate space for a string of maximum 20 chars  

        int j = 0;
        while(getline(fin2, line))
        {
            strcpy(array[j], line.c_str());

            char *buffer = strdup(array[j]);
            buffer[strlen(buffer) - 5] = '\0';  //delete file extension
            array[j] = buffer;
            
            char* substr = array[j] + 1;
            array[j] = substr;

            fout2 << substr << endl;
            j++;
        }
        fin2.close();
        fout2.close();
    }
    
    //show all students ID from StudentsID.txt
    void showStudentID(string a_filename)
    {
        fstream myfile(a_filename);
        string line; 

        while(getline(myfile, line))
        {
            cout << line << endl;
            numFiles++;
        }
        myfile.close();
    }

    //get total files submitted
    int getTotalFiles()
    {
        return numFiles;
    }
};

vector<int> encoding(string s1)
{
	//cout << "\n\nEncoding\n";
	unordered_map<string, int> table;
	for (int i = 0; i <= 255; i++) 
	{
		string ch = "";
		ch += char(i);
		table[ch] = i;
	}

	string p = "", c = "";
	p += s1[0];
	int code = 256;
	vector<int> output_code;
	// cout << "String\tOutput_Code\tAddition\n";
	for (int i = 0; i < s1.length(); i++) 
	{
		if (i != s1.length() - 1)
			c += s1[i + 1];
		if (table.find(p + c) != table.end()) 
			p = p + c;
		else 
		{
			//cout << p << "\t" << table[p] << "\t\t"
			//<< p + c << "\t" << code << endl;
			output_code.push_back(table[p]);
			table[p + c] = code;
			code++;
			p = c;
		}
		c = "";
	}
	//cout << p << "\t" << table[p] << endl;
	output_code.push_back(table[p]);
	return output_code;
}

class ReportGenerator 
{
protected:
    int del;
    int numOccurences;
    string filename;

public:
    ReportGenerator()
    {
        del = 0;
        numOccurences = 0;
    }

    //reset number of deleted comments
    void setDel(int a_del)
    {
        del = a_del;
    }
    
    //remove comment before code duplication checking
    string rem_cmt(string prgm)
    {
        int n = prgm.length();
        string res = "";

        // Flags to indicate that single line and multiple line comments
        // have started or not.
        bool s_cmt = false;
        
        // Traverse the given program
        for (int i = 0; i < n; i++)
        {
            /* If single line comment flag is on, then check for end of it
            if (s_cmt == true && prgm[i] == '\n')
                s_cmt = false;
            // If this character is in a comment, ignore it
            else if (s_cmt || m_cmt)
                continue;
            */
            // Check for beginning of comments and set the appropriate flags
            if(prgm[i] == '/' && prgm[i+1] == '/')
            {
                s_cmt = true; 
                m_cmt = false;
                del++;
                break;
            }
            else if(prgm[i] == '/' && prgm[i+1] == '*' && m_cmt == false)
            {
                m_cmt = true;  
                i++;
                del++;
            }
            // If multiple line comment is on, then check for end of it
            else if(prgm[i] == '*' && prgm[i+1] == '/')
            {   
                m_cmt = false;  
                i++;
                s_cmt = false;
            }
            // If current character is a non-comment character, append it to res
            else if(s_cmt == false && m_cmt == false)
                res += prgm[i];
        }
        return res;
    }

    //print number of deleted comments
    int printDel()
    {   
        return del;
    }

    //remove blank line before code duplication checking //not working properly
    void rem_blank_line(string a_filename)
    {   
        ifstream fin;   
        string line;
        string *lineArray = new string[1000];
        int lines = 0;
        
        fin.open(a_filename);
        
        while(getline(fin, line))
        {
            //Check if the line is empty
            if(!line.empty())
            {
                lineArray[lines] = line;
                lines++;
            }
        }
        fin.close();
        
        ofstream fout;                
        fout.open(a_filename); 
        for(int i = 0; i < lines; i++)
        {
            fout << lineArray[i];
        }  
        fout.close();
    }

    //remove unnecessary blank space before code duplication checking
    void rem_spaces(const char *a_filename)
    {
	    fstream outfile2("output3.cpp");
        FILE *input;
	    FILE *output;
	    char p;

	    input = fopen(a_filename, "r");
	    output = fopen("output3.cpp", "w");
	    while((p = getc(input)) != EOF)
	    {
		    //if(p != 32) //ASCII code for blank space is 32
		    if(p != 9) //ASCII code for horizontal tab is 9
		    if(p != '\n')
		    if(p != '/' && p != '*')
	    fputc(p, output);
	    }
        fclose(input);
        fclose(output);

        remove(a_filename);
        rename("output3.cpp", a_filename);
    }

    //converting string to code
    void decoding(vector<int> op)
    {
	    //cout << "\nDecoding\n";
	    unordered_map<int, string> table;
	    
        for (int i = 0; i <= 255; i++) 
	    {
		    string ch = "";
		    ch += char(i);
		    table[i] = ch;
	    }
	    int old = op[0], n;
	    string s = table[old];
	    string c = "";
	    c += s[0];
	    //cout << s;
	    int count = 256;
	    
        for (int i = 0; i < op.size() - 1; i++) 
	    {
		    n = op[i + 1];
		    if (table.find(n) == table.end()) 
		    {
			    s = table[old];
			    s = s + c;
		    }
		    else 
		    {
			    s = table[n];
		    }
		    //cout << s;
		    c = "";
		    c += s[0];
		    table[count] = table[old] + c;
		    count++;
		    old = n;
	    }
    }
};

class Interaction
{
private:

public:
    Interaction();

};

int main()
{
    // - - Implementing FileChecking Class - -
    FileChecking students;

    string path = "C:\\Users\\Lenovo\\OneDrive - mmu.edu.my\\Desktop\\Submission";

    cout << "\nReading all submitted files names: " << endl;
    students.readFileName(path);

    cout << "\nExtracting students ID from their file . . . . ." << endl;
    students.extractFileName(path);
    cout << "\nExtracting process completed . . . . ." << endl;
   
    cout << "\nShowing all students ID: " << endl;
    students.showStudentID("StudentsID.txt");

    cout << endl << "Number of files submitted: ";
    cout << students.getTotalFiles();

    int num = students.getTotalFiles();
    
    cout << endl << endl;

    fstream file;
    file.open("StudentsID.txt");
    string line; 
    char *array[num]; //to hold all student ID 
    char *a_array[num]; //to hold all of the modified files name

    for(int i = 0; i < num; i++)
        array[i] = (char*)malloc(31); //allocate space for a string of maximum 30 chars

    int j = 0;
    while(getline(file, line))
    {
        strcpy(array[j], line.c_str());
        j++;
    }
    
    printf("Name of modified file: \n"); 
    //duplicating original files for preserving purpose
    for(int i = 0; i < num; i++)
    {
        a_array[i] = (char*)malloc(31);  //allocate space for a string of maximum 30 chars
        char extension[10] = "x.cpp";

        sprintf(a_array[i], "%s%s", array[i], extension); //concatenate strings (modified files)
        cout << a_array[i] << endl;
    }
    cout << endl;

    // - - Implementing ReportGenerator Class - -
    ReportGenerator students1;
    fstream fin1, fout1;

    //remove comment
    for(int i = 0; i < num; i++)
    {
        char extension[10] = ".cpp";
        sprintf(array[i], "%s%s", array[i], extension); //concatenate strings (original files)

        ofstream outfile1(a_array[i]);
        fin1.open(array[i]);
        fout1.open(a_array[i]);
        string prg;
    
        while(getline(fin1, prg))
        {
            string ss1 =  students1.rem_cmt(prg);
            fout1 << ss1 << endl;
        }
        printf("Number of comments deleted for file[%d]: ", i);
        cout << students1.printDel() << endl; //count number of deletion in the file
        
        students1.setDel(0); //reset number of deletion
        fin1.close();
        fout1.close();
    }

    //remove blank spaces and blank line
    for(int i = 0; i < num; i++)
    {    
        //remove blank spaces
        students1.rem_spaces(a_array[i]);

        //remove blank line
        //students.rem_blank_line(a_array[i]); //not working
    }
    cout << endl;

    fstream fin2, fin3;
    int num2 = num * num;
    float array_marks[num2];
    char *array_files[num2];
    int order = 0;

    //calculating similarity score
    for(int q = 0; q < num; q++)
    {
        for(int w = 0; w < num; w++)
        {
            cout << endl << "Comparison between " << a_array[q] << " and " << a_array[w] << endl;
            array_files[order] = (char*)malloc(41);  //allocate space for a string of maximum 40 chars
            sprintf(array_files[order], "%s__%s", a_array[q], a_array[w]); //concatenate file names e.g. 1191101837.cpp__1191102014.cpp

            if(q == w)
            {
                cout << "Skipped" << endl;
                cout << "Similarity score for " << array_files[order] << " is invalid" << endl << endl;
            }
            else if(q != w)
            {
                fin2.open(a_array[q]);
                fin3.open(a_array[w]);
                cout << "- - Similarity checking in process - -" << endl << endl;
                string line2, line3;
                string *line2Array = new string[1000];
                string *line3Array = new string[1000];

                float count = 0;
                float size2 = 0;
                float size3 = 0;

                int x = 0;
                while(getline(fin2, line2))
                {
                    line2Array[x] = line2;
                    x++;
                }

                int y = 0;
                while(getline(fin3, line3))
                {
                    line3Array[y] = line3;
                    y++;
                }

                int z;
                if(x > y)
                    z = x;
                else    
                    z = y;

                //compare a string from file 1 with string in file 2 (one line by one line)
                for(int i = 0; i < z; i++)
                {
                    vector<int> output_code2 = encoding(line2Array[i]);
                    //cout << "Output Codes are: ";

                    for(int j = 0; j < output_code2.size(); j++) 
	                {
	                    //cout << output_code2[j] << " ";
                        size2++;
	                }
                    cout << endl;
                    students1.decoding(output_code2);

                    vector<int> output_code3 = encoding(line3Array[i]);
                    //cout << endl << "Output Codes are: ";

	                for (int j = 0; j < output_code3.size(); j++) 
	                {
		                //cout << output_code3[j] << " ";
                        size3++;
	                }
	                cout << endl;
	                students1.decoding(output_code3);
        
                    int a;
                    if(output_code2.size() > output_code3.size())
                        a = output_code2.size();
                    else    
                        a = output_code3.size();

                    for (int j = 0; j < a; j++) 
	                {
                         if(output_code2[j] == output_code3[j])
                            count++;
                    }
                    cout << endl;

                    cout << "- - Similarity checking completed - -" << endl;

                    cout << endl << "Total output codes in first file: " << size2;
                    cout << endl << "Total output codes in second file: " << size3;
                    cout << endl << "Number of occurrences: " << count; //intersection between two files - A ∩ B
                    cout << endl << "Total output codes involved: " << size2+size3; 
                    int size4 = size2 + size3 - count; //union between two files - A ∪ B

                    float cal = 0;
                    cal = count/size4; // Jaccard index Formula // J(A,B) = |A ∩ B|/|A ∪ B|

                    cout << endl << "Similarity score for " << array_files[order];
                    printf(" = %.2f", cal*100);
                    cout << " %" << endl << endl;
                    array_marks[order] = cal*100;
                    order++;
                } 
                fin2.close();
                fin3.close();
            }
        }
    }

    //display similarity score
    cout << endl << "- - Similarity Score - -" << endl;
    for(int i = 0; i < (num2 - num); i++)
    {
        cout << array_files[i] << " = ";
        printf("%.2f", array_marks[i]);
        cout << " %" << endl;
    }

    //insert data into SimScore.txt
    fstream fin4;
    ofstream myfile1("SimScore.txt");
    fin4.open("SimScore.txt");

    fin4 << "Students ID\t\t\t\t\tSimilarity Score" << endl;
    fin4 << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;
 
    for(int i = 0; i < (num2 - num); i++)
    {
        fin4 << array_files[i] << "\t\t" << fixed << setprecision(2) << array_marks[i] << " %" << endl;
    }
    fin4 << endl << "Total number of comparison: " << num2 - num << endl;
    fin4.close();

    //highlight top 10% of the most similar source code into TopSimScore.txt
    fstream fin5;
    ofstream myfile2("TopSimScore.txt");
    fin5.open("TopSimScore.txt");

    fin5 << "Top 10'%' of the most similar source code" << endl << endl;
    fin5 << "Students ID\t\t\t\t\tSimilarity Score" << endl;
    fin5 << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;

    float temp1;
    char *temp2;
    temp2 = (char*)malloc(sizeof(char)*41);

    int n = sizeof(array_marks)/sizeof(array_marks[0]); //same as num2

    //implementing bubble sort
    for(int i = n - 1; i > 0; i--)
    {
        for(int j = 0; j < i; j++)
        {
            if(array_marks[j] < array_marks[j+1])
            {
                temp1 = array_marks[j];
                array_marks[j] = array_marks[j+1];
                array_marks[j+1] = temp1;

                temp2 = array_files[j];
                array_files[j] = array_files[j+1];
                array_files[j+1] = temp2;
            } 
        }
    }  

    int k = (n - num)/2;
    int k1 = k*0.1;
    round(k1);

    for(int i = 0; i < k1; i++)
    {
        fin5 << array_files[i] <<  "\t\t" << fixed << setprecision(2) << array_marks[i] << " %" << endl;
    }
    fin5 << endl << "Actual total number of comparison: " << k << endl;
    fin5 << "Total files for 10'%' of the most similar source code: " << k1 << endl;
    fin5.close();

    //extra: listing all who plagiarize, those with similarity score above 10% 
    fstream fin6;
    ofstream myfile3("SimScoreAbove10.txt");
    fin6.open("SimScoreAbove10.txt");

    fin6 << "Plagiarism files detected (similarity score above 10'%')" << endl << endl;
    fin6 << "Students ID\t\t\t\t\tSimilarity Score" << endl;
    fin6 << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -" << endl;

    int p = 0;
    for(int i = 0; i < (num2 - num); i++)
    {
        if(array_marks[i] > 10)
        {
            fin6 << array_files[i] << "\t\t" << fixed << setprecision(2) << array_marks[i] << " %" << endl;
            p++;
        }
    }
    fin6 << endl << "Total number of plagiarism files: " << p << endl;
    fin6.close();

    return 0;
}
