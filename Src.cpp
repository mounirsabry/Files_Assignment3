#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>

using namespace std;

struct Course
{
    char course_ID[6];
    string name;
    string instructor_name;
    short weeks;

    friend istream& operator >> (istream&, Course&);
    friend ostream& operator << (ostream&, Course);
};
struct PrimaryIndex
{
    char course_ID[6];
    int offset;
};
struct InstructorNameFile
{
    string instructor_name;
    int RRN;
};
struct InstructorNameLinked
{
    char course_ID[6];
    int next_RRN;
};
//Main Functions.
bool Menu();
void AddCourse();
void DeleteByID(char ID []);
void DeleteByInstructor(string I);
void PrintByID();
void PrintByInstructor();
void PrintAll();
void UpdateByID(char ID []);
void UpdateByInstructor(string );
//Additional Functions.
bool IS_empty(ifstream&);
bool IS_exist(ifstream&);
void read_course(ifstream&, Course&);
void read_course(fstream&, Course&);
void write_course(ofstream&, const Course&);
void write_course(fstream&, const Course&);
void create_files();
void load_primary_index();
void load_instructor_name_index();
void save_primary_index();
void save_instructor_name_index();

void change_index_file_state(bool, fstream&);
void change_index_file_state(bool, ofstream&);

bool get_index_file_state(fstream&);
bool get_index_file_state(ifstream&);

void sort(vector<PrimaryIndex>&);
void sort(vector<InstructorNameFile>&);
int linear_search(vector<PrimaryIndex>&, const PrimaryIndex&);
int linear_search(vector<InstructorNameFile>&, const InstructorNameFile&);
int binary_search(vector<PrimaryIndex>&, const PrimaryIndex&);
int binary_search(vector<InstructorNameFile>&, const InstructorNameFile&);

const int MAX_BUFFER_SIZE = 200;
const int NAME_LINKED_RECORD_SIZE = 10;
const char DATA_FILE[] = "data.txt";
const char PRIMARY_INDEX[] = "primary.txt";
const char INSTRUCTOR_NAME_FILE[] = "instructor_name_file.txt";
const char INSTRUCTOR_NAME_LINKED[] = "instructor_name_linked.txt";

vector<PrimaryIndex> primary_table;
vector<InstructorNameFile> instructor_name_file_table;
vector<InstructorNameLinked> instructor_name_linked_table;

int main()
{
    create_files();
    load_primary_index();
    load_instructor_name_index();
    while (true)
    {
        if (Menu() == false)
        {
            break;
        }
//        cout << instructor_name_file_table.size() << " " << instructor_name_linked_table.size() << endl;
//        cout << "Primary Index." << endl;
//        for (PrimaryIndex temp : primary_table)
//        {
//            cout << temp.course_ID << " " << temp.offset << endl;
//        }
//        cout << "First File Index." << endl;
//        for (InstructorNameFile temp : instructor_name_file_table)
//        {
//            cout << temp.instructor_name << " " << temp.RRN << endl;
//        }
//        cout << "Second File Index." << endl;
//        for (InstructorNameLinked temp : instructor_name_linked_table)
//        {
//            cout << temp.course_ID << " " << temp.next_RRN << endl;
//        }
    }
    save_primary_index();
    save_instructor_name_index();
    system("Pause");
    return 0;
}

istream& operator >> (istream& in, Course& c)
{
    cout << "Enter The ID (5 Characters Max) :";
    if (cin.peek() == '\n')
    {
        cin.ignore(1000, '\n');
    }
    cin.getline(c.course_ID, 6, '\n');
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(1000, '\n');
    }

    cout << "Enter The Course name :";
    getline(in, c.name, '\n');

    cout << "Enter The Instructor Name :";
    getline(in, c.instructor_name, '\n');

    cout << "Enter The weeks :";
    in >> c.weeks;
    return in;
}
ostream& operator << (ostream& out, Course c)
{
    out << "__________________________________\nCourse ID : " << c.course_ID << endl
        << "Course Name : " << c.name << endl
        << "course Instructor : " << c.instructor_name << endl
        << "course weeks : " << c.weeks << endl << "______________________________________________\n";
    return out;
}
//Main Functions.
bool Menu()
{
    char c;
    cout << "1) Add New course \n" <<
         "2) Delete course (ID)\n"
         "3) Delete course (instructor name)\n" <<
         "4) Print course (ID)\n" <<
         "5) Print course (instructor name)\n" <<
//         "6) Print all courses\n" <<
         "7) update course (ID) \n" <<
         "8) update course (instructor name)\n" <<
         "9) Exit\n" <<
         "__________________________________________\n";

    while (true)
    {
        cout << "Please Enter Your Choice: ";
        cin >> c;
        if (cin.peek() != '\n')
        {
            cout << "Invalid Input." << endl;
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        if (c == '1')
        {
            AddCourse();
            return true;
        }
        else if (c == '2')
        {
            //get me your course id
            char ID [6] ;
            cout << "Enter The ID to be deleted :";
            if (cin.peek() == '\n')
            {
                cin.ignore(1000, '\n');
            }
            cin.getline(ID, 6, '\n');
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(1000, '\n');
            }
            DeleteByID(ID);
            return true;
        }
        else if (c == '3')
        {
            cout << "Enter instructor name to be deleted : \n" ;
            InstructorNameFile table ;

            cin.ignore(1000, '\n');

            if (cin.fail())
            {
                cin.clear();
                cin.ignore(1000, '\n');
            }
            getline (cin, table.instructor_name, '\n') ;
            int found_at = binary_search(instructor_name_file_table, table) ;
            if (found_at == -1)
            {
                cout << "Not found..\n" ;
                return 0;
            }
            else
            {
                string I  = table.instructor_name;
                DeleteByInstructor(I);
                return true;
            }
        }
        else if (c == '4')
        {

            PrintByID();
            return true;
        }
        else if (c == '5')
        {
            PrintByInstructor();
            return true;
        }
//        else if (c == '6')
//        {
//            PrintAll();
//            return true;
//        }
        else if (c == '7')
        {
            //get me your course id
            char ID [6] ;
            cout << "Enter The ID to be updated :";
            if (cin.peek() == '\n')
            {
                cin.ignore(1000, '\n');
            }
            cin.getline(ID, 6, '\n');
            if (cin.fail())
            {
                cin.clear();
                cin.ignore(1000, '\n');
            }
            UpdateByID(ID);
            return true;
        }
        else if (c == '8')
        {
            cout << "Note that you gotta insert new data for every course\n" ;
            cout << "Enter instructor name to be updated : \n" ;
            InstructorNameFile I ;
            cin.ignore(1000, '\n');

            if (cin.fail())
            {
                cin.clear();
                cin.ignore(1000, '\n');
            }
            getline (cin, I.instructor_name, '\n') ;
            int found_at = binary_search(instructor_name_file_table, I) ;
            if (found_at == -1)
            {
                cout << "Not found..\n" ;
                return 0;
            }
            else
            {
                string name = I.instructor_name ;

                UpdateByInstructor(name);
                return true;
            }

        }
        else if (c == '9')
        {
            return false;
        }
        else
        {
            cout << "Invalid Input." << endl;
        }
    }
}
void AddCourse()
{
    //Entering the new course and verify the course id.
    Course input;
    PrimaryIndex temp_primary;
    InstructorNameFile temp_name_file;
    InstructorNameLinked temp_name_linked;
    while (true)
    {
        cin >> input;
        strcpy(temp_primary.course_ID, input.course_ID);
        if (binary_search(primary_table, temp_primary) == -1)
        {
            break;
        }
        else
        {
            cout << "Error, a course with the same course ID already exists!!!" << endl
                 << "Enter another course." << endl;
        }
    }
    //Storing the new course.
    fstream data;
    data.open(DATA_FILE, ios::in | ios::out);
    if (data.fail())
    {
        cout << "Error, data file does not exist." << endl;
        cout << "Program Will Terminate." << endl;
        system("Pause");
        exit(1);
    }
    data.seekp(0, ios::end);
    int offset = data.tellp();
    write_course(data, input);
    //Adding it to the primary index.
    temp_primary.offset = offset;
    primary_table.push_back(temp_primary);
    sort(primary_table);
    //Adding it to the secondary index(instructor_name).
    temp_name_file.instructor_name = input.instructor_name;
    temp_name_file.RRN = -1; //Initial Value.
    int index = binary_search(instructor_name_file_table, temp_name_file);
    if (index == -1)
    {
        strcpy(temp_name_linked.course_ID, input.course_ID);
        temp_name_linked.next_RRN = -1;
        instructor_name_linked_table.push_back(temp_name_linked);

        temp_name_file.RRN = instructor_name_linked_table.size() - 1;
        instructor_name_file_table.push_back(temp_name_file);
    }
    else
    {
        temp_name_linked.next_RRN = instructor_name_file_table[index].RRN;
        strcpy(temp_name_linked.course_ID, input.course_ID);
        instructor_name_linked_table.push_back(temp_name_linked);

        instructor_name_file_table[index].RRN = instructor_name_linked_table.size() - 1;
        //temp_name_file.RRN = instructor_name_linked_table.size() - 1;
        //instructor_name_file_table.push_back(temp_name_file);
    }
    sort(instructor_name_file_table);
}
void DeleteByID(char ID [])
{
//
    //-------------------------------------------------------------------

    vector<PrimaryIndex>::iterator it = primary_table.begin(), next = primary_table.begin();
    bool found = 0 ;
    string copy_of_name ;
    int Num_records = primary_table.size() ; // we record them before any deletions
    int deleted_offset = 0  ;
    //--------------------------------------------------------------------
    // now search for it in the primary index
    for (PrimaryIndex temp : primary_table)
    {
        if (strcmp(temp.course_ID, ID ) == 0 && !found)
        {
            next = 1 + it ;
            deleted_offset = next->offset - it->offset ;

            cout << it->offset << " , " <<  "Deleted offset is : " << deleted_offset << " , " << next->offset << endl;

            primary_table.erase(it) ;
            found = 1 ;

            cout << "Book found..\n" ;

        }

        else if (found)      // shift offsets
        {
            cout << "I got here once\n" ;
            temp.offset -= deleted_offset ;
        }
        else
            it++ ;
    }

    if (!found)
    {
        cout << "Book not found.." ;
        return ;
    }
    else
    {
        bool got = 0 ;

        vector<Course> temp ;
        ifstream datafile (DATA_FILE, ios::in ) ;
        Course c ;
        for (int i = 0 ; i < Num_records ; i++)    // put them into a vector and rewrite
        {
            read_course(datafile, c) ;
            if (strcmp(c.course_ID, ID ) == 0 && !got )      // the right record found
            {
                got = 1 ;
                copy_of_name = c.instructor_name ;
                continue ;
            }
            else
            {
                temp.push_back(c) ;
//                cout << c << endl;
            }
        }
        datafile.close();
        // we're now ready to write the rest of courses
        ofstream f (DATA_FILE, ios::trunc) ;
        for (Course c : temp)
        {
            write_course(f, c)  ;
        }
        f.close() ;
        // here we finished compacting datafile
        // ______________________________________
        // modify the linked list
        InstructorNameFile myName ;
        myName.instructor_name = copy_of_name ;
        int found = 0 ;
        found = binary_search(instructor_name_file_table, myName) ;
        vector<InstructorNameFile>::iterator iter = instructor_name_file_table.begin();
        if(found != -1)  iter += found ;
        else {
            cout << "Book not found..\n" ;
            return ;
        }
        int temp_RRN = iter->RRN ;
//        instructor_name_file_table.erase(iter) ;

        vector<InstructorNameLinked>::iterator dummy = instructor_name_linked_table.begin() ;
        dummy += temp_RRN ;
        if (dummy->next_RRN == -1)
        {
            instructor_name_file_table.erase(iter) ;
            instructor_name_linked_table.erase(dummy) ;
            return ;
        }
        else
        {
        	got = 0 ;
			// 34an hyt8yr later
            if (strcmp(ID, dummy->course_ID) == 0  && !got)
            {
                temp_RRN = dummy->next_RRN ;
                instructor_name_linked_table.erase(dummy) ;
                vector<InstructorNameLinked>::iterator dummy = instructor_name_linked_table.begin() + temp_RRN ;

                iter->RRN = temp_RRN;
                cout << "iter->RRN" << iter->RRN << endl;
                got = 1 ;
            }
        }
    }
}
void DeleteByInstructor(string I)
{

    // 1- delete from sec index and inverted list
    // 2- delete and compact the datafile
    // 3- delete from PK
    //______________________________________

    InstructorNameFile table ;
    table.instructor_name = I ;
    vector<InstructorNameFile>::iterator it = instructor_name_file_table.begin();
    int found_at = binary_search(instructor_name_file_table, table) ;
    it += found_at ;
    int temp_RRN = it -> RRN ;

    instructor_name_file_table.erase(it) ;
    vector<InstructorNameLinked>::iterator dummy = instructor_name_linked_table.begin() ;
    dummy += temp_RRN ;
    int deleted_offset = 0  ;

    while (true)
    {
        if (temp_RRN == -1)
            break;
        else
        {

            //--------------------------------------------------------------------
            vector<PrimaryIndex>::iterator i = primary_table.begin(), next = primary_table.begin();
            char ID [6] ;
            strcpy(ID, dummy->course_ID ) ;
            bool id_found = 0 ;
            for (PrimaryIndex I : primary_table)  // now search for it in the primary index
            {
                if (strcmp(I.course_ID, ID ) == 0 && !id_found)
                {
                    next = 1 + i ;
                    deleted_offset = next->offset - i->offset ;
                    primary_table.erase(i) ;
                    id_found = 1 ;
                    i = next ;
                    cout << "Book found..\n" ;

                }

                else if (id_found)      // shift offsets
                {
//                    cout << "I got here once\n" ;
                    i->offset -= deleted_offset ;
                    i++ ;
                }
                else
                    i++ ;
            }
            cout << "I'm here isa\n" ;

            temp_RRN = dummy->next_RRN ;
            instructor_name_linked_table.erase(dummy) ;
            if (temp_RRN == -1)
                break ;
            dummy = instructor_name_linked_table.begin();
            dummy += temp_RRN ;

        }
    }

    //______________________________________
    vector<Course> temp_course ;
    ifstream datafile (DATA_FILE, ios::in ) ;
    Course c ;
    int Num_records = primary_table.size() ;
    for (int i = 0 ; i < Num_records ; i++)    // put them into a vector and rewrite
    {
        read_course(datafile, c) ;
        if (c.instructor_name == table.instructor_name )     // the right record found
        {
            continue ;
        }
        else
        {
            temp_course.push_back(c) ;
        }
    }
    datafile.close();
    // we're now ready to write the rest of courses
    ofstream f (DATA_FILE, ios::trunc) ;
    for (Course c : temp_course)
    {
        write_course(f, c)  ;
    }
    f.close() ;
    //______________________________________
}
void PrintByID()
{
    char ID[6];
    if (cin.peek() == '\n')
    {
        cin.ignore(1000, '\n');
    }
    while (true)
    {
        cout << "Enter The ID :";
        cin.getline(ID, 6, '\n');
        if (cin.fail())
        {
            cout << "Input exceeds variable size, please try again." << endl;
            cin.clear();
            cin.ignore(1000, '\n');
        }
        else if (strcmp(ID, "") == 0)
        {
            cout << "Input is empty, please try again." << endl;
        }
        else
        {
            break;
        }
    }
    PrimaryIndex temp_index;
    Course temp_course;
    strcpy(temp_index.course_ID, ID);
    temp_index.offset = 0; //Initial Value.

    int index = binary_search(primary_table, temp_index);
    if (index == -1)
    {
        cout << endl << "Course Not Found!!!!" << endl << endl;
    }
    else
    {
        int address = primary_table[index].offset;
        ifstream read;
        read.open(DATA_FILE);
        if (read.fail())
        {
            cout << "Error, data file does not exist." << endl
                 << "Program Will Terminate." << endl;
            system("Pause");
            exit(1);
        }
        read.seekg(address, ios::beg);
        read_course(read, temp_course);
        cout << temp_course;
    }
}
void PrintByInstructor()
{
    //Reading the Instructor Name from the user and verify it.
    string instructor;
    if (cin.peek() == '\n')
    {
        cin.ignore(1000, '\n');
    }
    while (true)
    {
        cout << "Enter The Instructor Name :";
        getline(cin, instructor, '\n');
        if (instructor == "")
        {
            cout << "Instructor Name is empty, please try again." << endl;
        }
        else
        {
            break;
        }
    }
    //Filling a vector with all courses_IDs from instructor_name_linked_table.
    vector<InstructorNameLinked> temp_vector;
    InstructorNameFile temp_name_file;
    InstructorNameLinked temp_name_linked;
    temp_name_file.instructor_name = instructor;
    temp_name_file.RRN = -1; //Initial Value.
    int index = binary_search(instructor_name_file_table, temp_name_file);
    if (index == -1)
    {
        cout << endl << "No course found with the given instructor name!!!" << endl << endl;
        return;
    }
    else
    {
        int linked_index = instructor_name_file_table[index].RRN;
        while (true)
        {
            strcpy(temp_name_linked.course_ID, instructor_name_linked_table[linked_index].course_ID);
            temp_name_linked.next_RRN = instructor_name_linked_table[linked_index].next_RRN;
            temp_vector.push_back(temp_name_linked);

            linked_index = temp_name_linked.next_RRN;
            if (linked_index == -1)
            {
                break;
            }
        }
    }
    //Filling a vector with the all primary indexes from primary_table.
    vector<PrimaryIndex> index_vector;
    PrimaryIndex temp_index;
    for (InstructorNameLinked I : temp_vector)
    {
        strcpy(temp_index.course_ID, I.course_ID);
        temp_index.offset = 0; //Initial Value.

        int index = binary_search(primary_table, temp_index);
        if (index == -1)
        {
            cout << "Vital Error, the record is missing from the primary index." << endl;
            return;
        }
        else
        {
            temp_index.offset = primary_table[index].offset;
            index_vector.push_back(temp_index);
        }
    }
    sort(index_vector);
    //Retrieving the courses data from the data file.
    ifstream read;
    read.open(DATA_FILE);
    if (read.fail())
    {
        cout << "Error, data file does not exist." << endl;
        cout << "Program will terminate." << endl;
        system("Pause");
        exit(1);
    }
    Course temp_course;
    for (PrimaryIndex temp : index_vector)
    {
        int offset = temp.offset;
        read.seekg(offset, ios::beg);
        read_course(read, temp_course);
        cout << temp_course;
    }
}
void PrintAll()
{
    ifstream read;
    read.open(DATA_FILE);
    read.seekg(0, ios::beg);

    Course temp;
    char character;
    while (true)
    {
        character = read.peek();
        if (character == EOF)
        {
            break;
        }
        read_course(read, temp);
        cout << temp;
    }
}
void UpdateByID(char ID [])
{
    DeleteByID(ID) ;
    AddCourse();
    sort(instructor_name_file_table);
    sort(primary_table) ;
}
void UpdateByInstructor(string I)
{
    DeleteByInstructor(I);
    cout << "How many courses you want to assign to this instructor : " ;
    int Num_courses ;
    cin >> Num_courses ;
    cout << "\n__________________________________________________\n" ;
    for (int i = 0 ; i < Num_courses ; i++)
    {
        AddCourse();
    }
}
//Additional Functions.
bool IS_empty(ifstream& check_file)
{
    check_file.seekg(0, ios::beg);
    return check_file.peek() == EOF;
}
bool IS_exist(ifstream& check_file)
{
    if (check_file.fail())
    {
        return false;
    }
    return true;
}
void read_course(ifstream& file, Course& temp)
{
    int len = 0;
    char buffer[MAX_BUFFER_SIZE];

    file.read((char*)&len, sizeof(len));
    file.read(buffer, len);

    istringstream stream(buffer);
    stream.getline(temp.course_ID, 6, '|');

    getline(stream, temp.name, '|');
    getline(stream, temp.instructor_name, '|');

    stream >> temp.weeks;
}
void read_course(fstream& file, Course& temp)
{
    int len = 0;
    char buffer[MAX_BUFFER_SIZE];

    file.read((char*)&len, sizeof(len));
    file.read(buffer, len);
    cout << len << endl;

    istringstream stream(buffer);
    stream.getline(temp.course_ID, 6, '|');

    getline(stream, temp.name, '|');
    getline(stream, temp.instructor_name, '|');

    stream >> temp.weeks;
}
void write_course(ofstream& file, const Course& temp)
{
    char buffer[MAX_BUFFER_SIZE];

    strcpy(buffer, temp.course_ID);
    strcat(buffer, "|");
    strcat(buffer, (temp.name.c_str()));
    strcat(buffer, "|");
    strcat(buffer, temp.instructor_name.c_str());
    strcat(buffer, "|");
    strcat(buffer, to_string(temp.weeks).c_str());
    strcat(buffer, "|");

    int len = strlen(buffer);
    file.write((char*)&len, sizeof(len));
    file.write(buffer, len);
}
void write_course(fstream& file, const Course& temp)
{
    char buffer[MAX_BUFFER_SIZE];

    strcpy(buffer, temp.course_ID);
    strcat(buffer, "|");
    strcat(buffer, (temp.name.c_str()));
    strcat(buffer, "|");
    strcat(buffer, temp.instructor_name.c_str());
    strcat(buffer, "|");
    strcat(buffer, to_string(temp.weeks).c_str());
    strcat(buffer, "|");

    int len = strlen(buffer);
    file.write((char*)&len, sizeof(len));
    file.write(buffer, len);
}
void create_files()
{
    ifstream check;
    //Checking Data_FILE.
    check.open(DATA_FILE);
    if (!IS_exist(check))
    {
        check.close();
        ofstream create;
        create.open(DATA_FILE);
        create.close();
    }
    else
    {
        check.close();
    }
    //Checking PRIMARY_INDEX.
    check.open(PRIMARY_INDEX);
    bool IS_recreate = false;
    if (!IS_exist(check) || IS_empty(check))
    {
        IS_recreate = true;
    }
    else
    {
        bool flag = get_index_file_state(check);
        if (flag == true)
        {
            IS_recreate = true;
        }
    }
    check.close();
    if (IS_recreate == true)
    {
        ifstream copy;
        copy.open(DATA_FILE);
        copy.seekg(0, ios::beg);

        Course temp_course;
        PrimaryIndex temp_index;
        char next_character;

        while(true)
        {
            next_character = copy.peek();
            if (next_character == EOF)
            {
                break;
            }
            int address = copy.tellg();
            read_course(copy, temp_course);

            strcpy(temp_index.course_ID, temp_course.course_ID);
            temp_index.offset = address;

            primary_table.push_back(temp_index);
        }
        sort(primary_table);
        save_primary_index();
    }
    //Checking INSTURCTOR_NAME_FILE.
    check.open(INSTRUCTOR_NAME_FILE);
    IS_recreate = false;
    if (!IS_exist(check) || IS_empty(check))
    {
        IS_recreate = true;
    }
    else
    {
        bool flag = get_index_file_state(check);
        if (flag == true)
        {
            IS_recreate = true;
        }
    }
    check.close();
    if (IS_recreate == true)
    {
        ifstream copy;
        copy.open(DATA_FILE);

        Course temp_course;
        InstructorNameFile temp_index;
        InstructorNameLinked temp_linked;
        char next_character;

        while (true)
        {
            next_character = copy.peek();
            if (next_character == EOF)
            {
                break;
            }
            read_course(copy, temp_course);

            temp_index.instructor_name = temp_course.instructor_name;
            temp_index.RRN = -1; //Initial Value.

            int index = linear_search(instructor_name_file_table, temp_index);
            if (index == -1)
            {
                strcpy(temp_linked.course_ID, temp_course.course_ID);
                temp_linked.next_RRN = -1;
                instructor_name_linked_table.push_back(temp_linked);

                temp_index.RRN = instructor_name_linked_table.size() - 1;
                instructor_name_file_table.push_back(temp_index);
            }
            else
            {
                strcpy(temp_linked.course_ID, temp_course.course_ID);
                temp_linked.next_RRN = instructor_name_file_table[index].RRN;
                instructor_name_linked_table.push_back(temp_linked);

                instructor_name_file_table[index].RRN = instructor_name_linked_table.size() - 1;
            }
        }
        sort(instructor_name_file_table);
        save_instructor_name_index();
    }
}
void load_primary_index()
{
    //Clear the previous data in the vector.
    primary_table.clear();
    //Filling the vector from the file.
    ifstream read;
    read.open(PRIMARY_INDEX);
    if (read.fail())
    {
        cout << "Error Opening Primary File." << endl
             << "Program Will Terminate." << endl;
        system("Pause");
        exit(1);
    }
    read.seekg(1, ios::beg);
    PrimaryIndex temp;
    char next_character = 0;
    while (true)
    {
        next_character = read.peek();
        if (next_character == EOF)
        {
            break;
        }
        read.read(temp.course_ID, sizeof(temp.course_ID));
        read.read((char*)&temp.offset, sizeof(temp.offset));
        primary_table.push_back(temp);
    }
    sort(primary_table);
}
void load_instructor_name_index()
{
    //Clearing the previous data in the vectors.
    instructor_name_file_table.clear();
    instructor_name_linked_table.clear();
    //Filling the vectors from the index files.
    ifstream read_file;
    ifstream read_linked;

    read_file.open(INSTRUCTOR_NAME_FILE);
    read_linked.open(INSTRUCTOR_NAME_LINKED);
    if (read_file.fail() || read_linked.fail())
    {
        cout << "Error Opening Name Index Files" << endl
             << "Program Will Terminate." << endl;
        system("Pause");
        exit(1);
    }

    InstructorNameFile temp_index;
    InstructorNameLinked temp_linked;
    char next_character;

    read_file.seekg(1, ios::beg);
    while (true)
    {
        next_character = read_file.peek();
        if (next_character == EOF)
        {
            break;
        }
        short length = 0;
        read_file.read((char*)&length, sizeof(length));
        temp_index.instructor_name.resize(length);
        read_file.read((char*)&temp_index.instructor_name[0], length);
        read_file.read((char*)&temp_index.RRN, sizeof(temp_index.RRN));

        instructor_name_file_table.push_back(temp_index);
    }
    while (true)
    {
        next_character = read_linked.peek();
        if (next_character == EOF)
        {
            break;
        }
        read_linked.read(temp_linked.course_ID, sizeof(temp_linked.course_ID));
        read_linked.read((char*)&temp_linked.next_RRN, sizeof(temp_linked.next_RRN));

        instructor_name_linked_table.push_back(temp_linked);
    }
}
void save_primary_index()
{
    ofstream trunc;
    trunc.open(PRIMARY_INDEX, ios::trunc);
    change_index_file_state(true, trunc);
    //Writing Vector to File.
    for (PrimaryIndex temp : primary_table)
    {
        trunc.write(temp.course_ID, sizeof(temp.course_ID));
        trunc.write((char*)&temp.offset, sizeof(temp.offset));
    }
    change_index_file_state(false, trunc);
    trunc.close();
}
void save_instructor_name_index()
{
    ofstream trunc;
    trunc.open(INSTRUCTOR_NAME_FILE, ios::trunc);
    change_index_file_state(true, trunc);
    trunc.close();

    trunc.open(INSTRUCTOR_NAME_LINKED, ios::trunc);
    trunc.close();

    fstream writing;
    writing.open(INSTRUCTOR_NAME_FILE, ios::in | ios::out);
    writing.seekp(1, ios::beg);
    //Writing First File.
    for (InstructorNameFile temp : instructor_name_file_table)
    {
        short length = temp.instructor_name.length();
        writing.write((char*)&length, sizeof(length));
        writing.write((char*)&temp.instructor_name[0], length);
        writing.write((char*)&temp.RRN, sizeof(temp.RRN));
    }
    writing.close();
    //Writing Second File.
    writing.open(INSTRUCTOR_NAME_LINKED, ios::in | ios::out);
    for (InstructorNameLinked temp : instructor_name_linked_table)
    {
        writing.write(temp.course_ID, sizeof(temp.course_ID));
        writing.write((char*)&temp.next_RRN, sizeof(temp.next_RRN));
    }
    writing.close();

    fstream change_state;
    change_state.open(INSTRUCTOR_NAME_FILE, ios::out | ios::in);
    change_index_file_state(false, change_state);
}
void change_index_file_state(bool state, fstream& change)
{
    change.seekp(0, ios::beg);
    change << state;
}
void change_index_file_state(bool state, ofstream& change)
{
    change.seekp(0, ios::beg);
    change << state;
}
bool get_index_file_state(fstream& get)
{
    get.seekg(0, ios::beg);
    bool flag;
    get >> flag;
    if (flag == false)
    {
        return false;
    }
    return true;
}
bool get_index_file_state(ifstream& get)
{
    get.seekg(0, ios::beg);
    bool flag;
    get >> flag;
    if (flag == false)
    {
        return false;
    }
    return true;
}
//Sorting and Searching.
void sort(vector<PrimaryIndex>& table)
{
    //Insertion Sort.
    for (int i = 1; i < table.size(); i++)
    {
        for (int j = i; j > 0 &&
                strcmp(table[j].course_ID, table[j - 1].course_ID) < 0; j--)
        {
            PrimaryIndex temp = table[j - 1];
            table[j - 1] = table[j];
            table[j] = temp;
        }
    }
}
void sort(vector<InstructorNameFile>& table)
{
    //Insertion Sort.
    for (int i = 1; i < table.size(); i++)
    {
        for (int j = i; j > 0 &&
                table[j].instructor_name < table[j - 1].instructor_name; j--)
        {
            InstructorNameFile temp = table[j - 1];
            table[j - 1] = table[j];
            table[j] = temp;
        }
    }
}
int linear_search(vector<PrimaryIndex>& table, const PrimaryIndex& key)
{
    for (int i = 0; i < table.size(); i++)
    {
        if (strcmp(table[i].course_ID, key.course_ID) == 0)
        {
            return i;
        }
    }
    return -1;
}
int linear_search(vector<InstructorNameFile>& table, const InstructorNameFile& key)
{
    for (int i = 0; i < table.size(); i++)
    {
        if (table[i].instructor_name == key.instructor_name)
        {
            return i;
        }
    }
    return -1;
}
int binary_search(vector<PrimaryIndex>& table, const PrimaryIndex& key)
{
    int low = 0;
    int high = table.size() - 1;
    while (low <= high)
    {
        int mid = (low + high) / 2;
        if (strcmp(key.course_ID, table[mid].course_ID) == 0)
        {
            return mid;
        }
        else if (strcmp(key.course_ID, table[mid].course_ID) > 0)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    return -1;
}
int binary_search(vector<InstructorNameFile>& table, const InstructorNameFile& key)
{
    int low = 0;
    int high = table.size() - 1;
    while (low <= high)
    {
        int mid = (low + high) / 2;
        if (key.instructor_name == table[mid].instructor_name)
        {
            return mid;
        }
        else if (key.instructor_name > table[mid].instructor_name)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    return -1;
}
