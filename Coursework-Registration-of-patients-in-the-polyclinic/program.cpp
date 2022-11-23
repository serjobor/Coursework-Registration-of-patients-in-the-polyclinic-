// КП по САОД (прога).cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//Задание на КП:
/*
 Вариант: 759 - поликлинника
 -Предметная область (3)- Регистрация больных в поликлинике (см. п. 9.4) 
 -Метод хеширования (3)- Закрытое хеширование с двойным хешированием     | ВЫПОЛНЕНО 
 -Метод сортировки (3)- Шейкерная                                        | ВЫПОЛНЕНО
 -Вид списка (4)- Слоеный                                                | ВЫПОЛНЕНО
 -Метод обхода дерева(0)- Симметричный                                   | ВЫПОЛНЕНО                  
 -Алгоритм поиска слова в тексте(1)- Прямой                              | ВЫПОЛНЕНО 
 */

#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <iomanip>    

using namespace std;

const int size_of_key = 9;
const int size_of_spreadsheet = 100;

string path_to_patient = "D:\\Учеба\\4 семестр\\КП по САОД\\КП по САОД (прога)\\Пациенты.txt";
string path_to_doctors = "D:\\Учеба\\4 семестр\\КП по САОД\\КП по САОД (прога)\\Врачи.txt";
string path_to_referral = "D:\\Учеба\\4 семестр\\КП по САОД\\КП по САОД (прога)\\Направления.txt";

struct Patient
{
    Patient()
    {
        RegistrationNumber = "\0";
        FIO = "\0";
        Address = "\0";
        Place_of_Work_or_Study = "\0";
        Year_of_birth = 0;
    }

    string RegistrationNumber;    // Регистрационный номер пациента
    string FIO;                   // ФИО пациента
    string Address;               // Адрес пациента
    string Place_of_Work_or_Study;// Место работы (учебы)пациента
    int Year_of_birth;            // Год рождения пациента

    static int id;

    void SetInfo(string reg, string fio, string ad, string pl, int year)
    {
        RegistrationNumber = reg;
        FIO = fio;
        Address = ad;
        Place_of_Work_or_Study = pl;
        Year_of_birth = year;
    }
    void SetZeroes()
    {
        RegistrationNumber = "\0";
        FIO = "\0";
        Address = "\0";
        Place_of_Work_or_Study = "\0";
        Year_of_birth = 0;
    }

    Patient& operator = (const Patient& other)
    {
        this->RegistrationNumber = other.RegistrationNumber;
        this->FIO = other.FIO;
        this->Address = other.Address;
        this->Place_of_Work_or_Study = other.Place_of_Work_or_Study;
        this->Year_of_birth = other.Year_of_birth;
        return *this;
    }
};

struct Doctor
{
    Doctor()
    {
        FIO = "\0"; // до 25 символов
        Position = "\0";
        Admission_Schedule = "\0";
        Cabinet_Number = 0;

        left = right = 0;
        height = 1;
    }

    string FIO;// до 25 символов
    string Position;
    string Admission_Schedule; // график приема
    int Cabinet_Number;

    // для дерева:
    unsigned char height;
    Doctor* left;
    Doctor* right;

    void SetInfo(string fio, string position, string admission_schedule, int cabinet_number)
    {
        FIO = fio;
        Position = position;
        Admission_Schedule = admission_schedule;
        Cabinet_Number = cabinet_number;
    }
    void SetInfo(Doctor& other)
    {
        FIO = other.FIO;
        Position = other.Position;
        Admission_Schedule = other.Admission_Schedule;
        Cabinet_Number = other.Cabinet_Number;
    }
    void SetZeroes()
    {
        FIO = "\0";
        Position = "\0";
        Admission_Schedule = "\0";
        Cabinet_Number = 0;

        left = right = 0;
        height = 1;
    }

    Doctor& operator = (const Doctor& other)
    {
        this->FIO = other.FIO;
        this->Position = other.Position;
        this->Admission_Schedule = other.Admission_Schedule;
        this->Cabinet_Number = other.Cabinet_Number;
        return *this;
    }
};

struct Referral_to_Doctor
{
    Referral_to_Doctor()
    {
        RegistrationNumber_Patient = "\0";
        FIO_Doctor = "\0";
        Referral_time = "\0";
        Referral_date = "\0";

        Next = nullptr;
        Next_block = nullptr;
    }
    Referral_to_Doctor(string RegistrationNumber_Patient, string FIO_Doctor, string Referral_time, string Referral_date)
    {
        this->RegistrationNumber_Patient = RegistrationNumber_Patient;
        this->FIO_Doctor = FIO_Doctor;
        this->Referral_time = Referral_time;
        this->Referral_date = Referral_date;

        Next = nullptr;
        Next_block = nullptr;
    }

    string RegistrationNumber_Patient;
    string FIO_Doctor;
    string Referral_time;
    string Referral_date;

    //для слоеного списка
    Referral_to_Doctor* Next;
    Referral_to_Doctor* Next_block;

    Referral_to_Doctor& operator = (const Referral_to_Doctor& other)
    {
        this->RegistrationNumber_Patient = other.RegistrationNumber_Patient;
        this->FIO_Doctor = other.FIO_Doctor;
        this->Referral_time = other.Referral_time;
        this->Referral_date = other.Referral_date;
        return *this;
    }
};

void Menu()
{
    system("cls");
    cout << " _______________________________________________________________" << endl;
    cout << "/                            Меню                               \\"<< endl;
    cout << "|---------------------------------------------------------------|" << endl;
    cout << "|Пациенты поликлиники:                                          |" << endl;
    cout << "|Нажмите 1, чтобы зарегистрировать больного.                    |" << endl;
    cout << "|Нажмите 2, чтобы просмотреть список зарегестрированных больных.|" << endl;
    cout << "|Нажмите 3, чтобы найти больного по ФИО.                        |" << endl;
    cout << "|Нажмите 4, чтобы найти больного по регистрационному номеру.    |" << endl;
    cout << "|Нажмите 5, чтобы удалить больного.                             |" << endl;
    cout << "|Нажмите 6, чтобы очистить базу данных больных.                 |" << endl;
    cout << "|---------------------------------------------------------------|" << endl;
    cout << "|Врачи поликлиники:                                             |" << endl;
    cout << "|Нажмите 7, чтобы добавить нового врача.                        |" << endl;
    cout << "|Нажмите 8, чтобы просмотреть список врачей.                    |" << endl;
    cout << "|Нажмите 9, чтобы найти врача по ФИО.                           |" << endl;
    cout << "|Нажмите 10, чтобы найти врача по фрагменту Должность.          |" << endl;
    cout << "|Нажмите 11, чтобы удалить сведенья о враче.                    |" << endl;
    cout << "|Нажмите 12, чтобы очистить базу данных врачей.                 |" << endl;
    cout << "|---------------------------------------------------------------|" << endl;
    cout << "|Нажмите 13, чтобы выдать пациенту направление:                 |" << endl;
    cout << "|Нажмите 14, чтобы просмотреть выданные направления:            |" << endl;
    cout << "|---------------------------------------------------------------|" << endl;
    cout << "|Нажмите 15, чтобы возвратить направление у пациента:           |" << endl;
    cout << "|---------------------------------------------------------------|" << endl;
    cout << "|Нажмите 0, чтобы выйти из программы.                           |" << endl;
    cout << "\\_______________________________________________________________/"<< endl << endl;
}

int Int_check(string text)
{
    while (true)
    {
        int value;
        cout << text;
        cin >> value;
                        
        if (cin.fail() || cin.get() != '\n')
        {
            cout << "Ошибка! Повторите ввод!" << endl;;
            cin.clear();
            cin.ignore(32767, '\n');
            continue;
        }       

        if (text == "Выберете пункт меню: ")
        {
            if (value < 0 || value > 15)
            {
                cout << "Ошибка! Повторите ввод!" << endl;
                continue;
            }
            return value;
        }

        if (text == "Введите количество новых больных, которых хотите занести в базу данных поликлиники: ")
        {
            if (value < 0)
            {
                cout << "Ошибка! Повторите ввод!" << endl << endl;
                continue;
            }
            return value;
        }

        if (text == "Введите год рождения больного: ")
        {
            if (value < 1900 or value > 2022)
            {
                cout << "Ошибка! Повторите ввод!" << endl << endl;
                continue;
            }
            return value;
        }

        if (text == "Введите номер участка (1-9): ")
        {
            if (value < 1 or value > 9)
            {
                cout << "Ошибка! Повторите ввод!" << endl << endl;
                continue;
            }
            return value;
        }

        if (text == "Введите номер кабинета доктора(1-24): ")
        {
            if (value < 1 or value > 24)
            {
                cout << "Ошибка! Повторите ввод!" << endl << endl;
                continue;
            }
            return value;
        }

        if (text == "Введите день: ")
        {
            if (value < 1 or value > 31)
            {
                cout << "Ошибка! Повторите ввод!" << endl << endl;
                continue;
            }
            return value;
        }

        if (text == "Введите месяц: ")
        {
            if (value < 1 or value > 12)
            {
                cout << "Ошибка! Повторите ввод!" << endl << endl;
                continue;
            }
            return value;
        }

        if (text == "Введите год: ")
        {
            if (value != 2022)
            {
                cout << "Ошибка! Повторите ввод!" << endl << endl;
                continue;
            }
            return value;
        }
        return value;
    }
}

void Name_check(string& name, string text) // Проверка на имя 
{
    while (true)
    {
        cout << text;
        getline(cin, name);
        bool incorrect = false;

        if (name.length() > 25)
        {
            cout << "Ошибка! Повторите ввод" << endl;
            continue;
        }

        if (name[0] == '.' or name[0] == '-')
        {
            cout << "[" << name[0] << "]" << " - Недопустимый символ!" << endl;
            continue;
        }

        if (int('а') <= int(name[0]) && int(name[0]) <= int('я'))
        {
            cout << "[" << name[0] << "]" << " - Недопустимый символ!" << endl;
            continue;
        }

        int check = 0;
        for (int i = 0; i < name.length(); i++)
        {
            if (int('А') <= int(name[i]) && int(name[i]) <= int('Я') || int('а') <= int(name[i]) && int(name[i]) <= int('я') ||
                int(name[i]) == int(' ') || int(name[i]) == int('.') || int(name[i]) == int('ё') || int(name[i]) == int('Ё') ||
                int(name[i]) == int('-'))
            {
                
            }
            else
            {
                cout << name[i] << " - Недопустимый символ!" << endl;
                incorrect = true;
                break;
            }
        }

        if (incorrect)
        {
            continue;
        }

        else
        {
            break;
        }
    }
}

void Adress_check(string& ad, string text)
{
    while (true)
    {
        cout << text;
        getline(cin, ad);
        bool incorrect = false;

        if (ad.length() > 50)
        {
            cout << "Ошибка! Повторите ввод" << endl << endl;
            continue;
        }

        if (ad[0] == '.' or ad[0] == '-')
        {
            cout << "[" << ad[0] << "]" << " - Недопустимый символ!" << endl;
            continue;
        }

        int check = 0;
        for (size_t i = 0; i < ad.length(); i++)
        { 
            if (int('А') <= int(ad[i]) && int(ad[i]) <= int('Я') || int('а') <= int(ad[i]) && int(ad[i]) <= int('я') ||
                int('0') <= int(ad[i]) && int(ad[i]) <= int('9') ||
                int(ad[i]) == int('.') || int(ad[i]) == int(' ') ||
                int(ad[i]) == int('ё') || int(ad[i]) == int('Ё') ||
                int(ad[i]) == int('/') || int(ad[i]) == int('-'))
            {

            }
            else
            {
                cout << ad[i] << " - Недопустимый символ!" << endl;
                incorrect = true;
                break;
            }
        }

        if (incorrect)
        {
            continue;
        }

        else
        {
            break;
        }
    }
}

void Place_check(string& pl, string text)
{
    while (true)
    {
        cout << text;
        getline(cin, pl);
        bool incorrect = false;

        if (pl.length() > 50)
        {
            cout << "Ошибка! Повторите ввод" << endl << endl;
            continue;
        }

        if (pl[0] == '.' or pl[0] == '-')
        {
            cout << "[" << pl[0] << "]" << " - Недопустимый символ!" << endl;
            continue;
        }

        int check = 0;
        for (size_t i = 0; i < pl.length(); i++)
        { 
            if (int('А') <= int(pl[i]) && int(pl[i]) <= int('Я') || int('а') <= int(pl[i]) && int(pl[i]) <= int('я') ||
                int('0') <= int(pl[i]) && int(pl[i]) <= int('9') ||
                int(pl[i]) == int('.') || int(pl[i]) == int(' ') ||
                int(pl[i]) == int('ё') || int(pl[i]) == int('Ё') ||
                int(pl[i]) == int('/') || int(pl[i]) == int('-'))
            {

            }

            else
            {
                cout << pl[i] << " - Недопустимый символ!" << endl;
                incorrect = true;
                break;
            }
        }

        if (incorrect)
        {
            continue;
        }

        else
        {
            break;
        }
    }
}

int Patient::id = 0;

void Filling_Reg_Number(string& RegNumber)
{
    cout << "Заполнение Регистрационного номера: " << endl << endl;

    int district;
    district = Int_check("Введите номер участка (1-9): ");
    string str_district = to_string(district);

    int number;    
    number = Patient::id;
    Patient::id++;

    string str_number = to_string(number);

    RegNumber = "0" + str_district + "-";

    for (int i = 0; i < 6 - str_number.length(); i++)
    {
        RegNumber += "0";
    }

    RegNumber += str_number;
}

void Reg_Number_check(string& RegNumber, string text) // Проверка на имя 
{
    while (true)
    {
        cout << text;
        getline(cin, RegNumber);
        bool incorrect = false;

        if (RegNumber.length() > 9)
        {
            cout << "Ошибка! Повторите ввод" << endl << endl;
            continue;
        }
        if (RegNumber[0] != '0')
        {
            cout << "Ошибка! Повторите ввод" << endl << endl;
            continue;
        }
        if (RegNumber[2] != '-')
        {
            cout << "Ошибка! Повторите ввод" << endl << endl;
            continue;
        }

        int check = 0;
        for (size_t i = 0; i < RegNumber.length(); i++)
        { 
            if (int('0') <= int(RegNumber[i]) && int(RegNumber[i]) <= int('9') ||
                int(RegNumber[i]) == int('-'))
            {

            }
            else
            {
                cout << RegNumber[i] << " - Недопустимый символ!" << endl;
                incorrect = true;
                break;
            }
        }

        if (incorrect)
        {
            continue;
        }

        else
        {
            break;
        }
    }
}

void infoAboutPatient(Patient& new_patient)
{
    string  FIO, Place_of_Work, adress, Reg_Number;
    int birth;
    Name_check(FIO, "Введите ФИО больного: ");
    cout << endl;
    birth = Int_check("Введите год рождения больного: ");
    cout << endl;
    Adress_check(adress, "Введите адрес проживания больного: ");
    cout << endl;
    Place_check(Place_of_Work, "Введите место работы(учебы) больного: ");
    cout << endl;

    Filling_Reg_Number(Reg_Number);

    new_patient.SetInfo(Reg_Number,FIO, adress, Place_of_Work, birth);
}

int HashFunction(string key)
{
    unsigned int hash = 0;
    unsigned int kod = 0;

    for (int i = 1; i <= size_of_key; i++)
    {
        kod = int(key[i]) * (i * 99);
        hash += 11 * (pow(kod, 3));
    }

    hash %= size_of_spreadsheet;

    return hash;
}

int HashFunction2(string key)
{
    unsigned int hash = 0;

    for (int i = 1; i <= size_of_key; i++)
    {
        hash = int(key[i])*5+i*7;
    }    
    hash %= size_of_spreadsheet;

    if (hash % 10 == 5 )
    {
        hash = hash + 2;
    }

    if (hash % 2 == 0)
    {
        hash = hash + 1;
    }

    return hash;
}

bool FillHashSpreadsheet(Patient* array_of_patient, Patient& new_patient, int hashed_key)
{
    bool key_setted = false;

    if (array_of_patient[hashed_key].RegistrationNumber == "\0" or array_of_patient[hashed_key].RegistrationNumber == "delete")
    {
        array_of_patient[hashed_key] = new_patient;
        key_setted = true;
        
        cout << endl;
        cout << "Регистрационный номер добавлен." << endl << endl;
        return key_setted;
    }

    else
    {
        int hashed_key_2 = HashFunction2(new_patient.RegistrationNumber);
        int adress;

        for (int i = 0; i < 100; i++)
        {
            adress = (hashed_key + hashed_key_2 * i) % 100;
            if (array_of_patient[adress].RegistrationNumber == "\0" or array_of_patient[adress].RegistrationNumber == "delete")
            {
                array_of_patient[adress] = new_patient;
                key_setted = true;
                cout << endl;
                cout << "Регистрационный номер добавлен." << endl << endl;
                return key_setted;
            }
        }
             
    }
    cout << endl;
    cout << "Регистрационный номер НЕ добавлен." << endl << endl;
    return key_setted;
}

void SwowSpreadsheet(Patient* spreadsheet_of_patient)
{
    int number = 1;

    cout << "'Список зарегестрированных пациентов больницы'" << endl << endl;
    cout << "N  |Регистрационный номер|             ФИО пациента|" << endl;
    cout << "---|---------------------|-------------------------|" << endl;

    for (int i = 0; i < size_of_spreadsheet; i++)
    {
        if (spreadsheet_of_patient[i].Year_of_birth !=0)
        {
            cout << setw(3) << number << "|" << setw(21) << spreadsheet_of_patient[i].RegistrationNumber << "|" << setw(25) << spreadsheet_of_patient[i].FIO << "|"<< endl;
            number++;
        }
    }
    cout << endl;
}

void Find_Patient_FIO(Patient* spreadsheet_of_patient)
{
    string  FIO;
    cout << "'Поиск пациента по ФИО'" << endl << endl;

    Name_check(FIO, "Введите ФИО больного: ");
    cout << endl << endl;

    int amount = 1;
    int found = 0;
    cout << "Список найденных больных: " << endl << endl;

    for (int i = 0; i < size_of_spreadsheet; i++)
    {
        if (spreadsheet_of_patient[i].FIO == FIO)
        {
            found++;
            if (found == 1)
            {
                cout << endl << endl;
                cout << "N  |Регистрационный номер|             ФИО пациента|" << endl;
                cout << "---|---------------------|-------------------------|" << endl;
                found++;
            }          
            
            cout << setw(3) << amount << "|" << setw(21) << spreadsheet_of_patient[i].RegistrationNumber << "|" << setw(25) << spreadsheet_of_patient[i].FIO << "|" << endl;
            amount++;
        }
    }

    if (found == 0)
    {
        cout << "Данного пациента в списке нет." << endl << endl;
    }
    cout << endl;
}

int Find_Patient_RegNumber(Patient* spreadsheet_of_patient, string Regnumber) 
{
    int hashed_key = HashFunction(Regnumber);
    int adress = hashed_key;

    if (spreadsheet_of_patient[adress].RegistrationNumber == Regnumber)
    {
        return adress;
    }

    if (spreadsheet_of_patient[adress].RegistrationNumber == "\0")
    {
        return size_of_spreadsheet;
    }
   
    else
    {
        int hashed_key_2 = HashFunction2(Regnumber);
        int adress_2;

        for (int i = 0; i < 100; i++)
        {
            adress_2 = (hashed_key + hashed_key_2 * i) % 100;

            if (spreadsheet_of_patient[adress_2].RegistrationNumber == Regnumber)
            {
                return adress_2;
            }

            if (spreadsheet_of_patient[adress_2].RegistrationNumber == "\0")
            {
                return size_of_spreadsheet;
            }
        }
    }
    return size_of_spreadsheet;
}

bool Can_delete_Patient(Referral_to_Doctor* start, string Reg_Number)
{
    while (start != nullptr)
    {
        if (start->RegistrationNumber_Patient == Reg_Number)
        {
            return false;
        }
        start = start->Next;
    }
    return true;
}

bool Can_delete_Doctor(Referral_to_Doctor* start, string FIO)
{
    while (start != nullptr)
    {
        if (start->FIO_Doctor == FIO)
        {
            return false;
        }

        if (start->FIO_Doctor[0] != FIO[0])
        {
            start = start->Next_block;
        }

        else
        {
            start = start->Next;
        }
    }
    return true;
}

void Clear_Patient_FIO(Patient* spreadsheet_of_patient, Referral_to_Doctor* start)
{
    string  FIO;
    cout << "'Удаление пациента по ФИО'" << endl << endl;

    Name_check(FIO, "Введите ФИО больного: ");
    cout << endl << endl;

    int patient_to_delete = 0;

    for (int i = 0; i < size_of_spreadsheet; i++)
    {
        if (spreadsheet_of_patient[i].FIO == FIO)
        {
            patient_to_delete++;
        }
    }

    if (patient_to_delete == 0)
    {
        cout << "Данного пациента в списке нет." << endl << endl;
        return;
    }

    if (patient_to_delete == 1)
    {
        for (int i = 0; i < size_of_spreadsheet; i++)
        {
            if (spreadsheet_of_patient[i].FIO == FIO)
            {
                cout << "Список найденных больных: " << endl << endl;
                cout << "N  |Регистрационный номер|             ФИО пациента|" << endl;
                cout << "---|---------------------|-------------------------|" << endl;
                cout << "  1|" << setw(21) << spreadsheet_of_patient[i].RegistrationNumber << "|" << setw(25) << spreadsheet_of_patient[i].FIO << "|"<< endl;
            
                if (Can_delete_Patient(start, spreadsheet_of_patient[i].RegistrationNumber))
                {
                    spreadsheet_of_patient[i].SetZeroes();
                    cout << "Данные о пациенте успешно удалены " << endl;
                    return;
                }
                
                cout << endl << "Данному пациенту выдано направление! Удаление невозможно!" << endl << endl;
                return;
            }
        }
    }

    cout << "В базе хранится несколько пациентов с таким ФИО" << endl << endl;
    int number_to_delete = 1;

    cout << "N  |Регистрационный номер|             ФИО пациента|" << endl;
    cout << "---|---------------------|-------------------------|" << endl;
    
    for (int i = 0; i < size_of_spreadsheet; i++)
    {
        if (spreadsheet_of_patient[i].FIO == FIO)
        {
            cout << setw(3) << number_to_delete << "|" << setw(21) << spreadsheet_of_patient[i].RegistrationNumber << "|" << setw(25) << spreadsheet_of_patient[i].FIO << "|" << endl;            
            number_to_delete++;       
        }
    }
    cout << endl; 

    while (true)
    {
        number_to_delete = Int_check("Выберите порядковый номер пациента, которого хотите удалить:");

        if (number_to_delete > patient_to_delete || number_to_delete<=0)
        {
            cout << "Ошибка ввода! Повторите ввод!" << endl << endl;
            continue;
        }
        break;
    }

    int compare_to_delete = 1;

    for (int i = 0; i < size_of_spreadsheet; i++)
    {
        if (spreadsheet_of_patient[i].FIO == FIO)
        { 
            if (compare_to_delete == number_to_delete)
            {
                if (Can_delete_Patient(start, spreadsheet_of_patient[i].RegistrationNumber))
                {
                    spreadsheet_of_patient[i].SetZeroes();
                    cout << "Данные о пациенте успешно удалены " << endl;
                    return;
                }

                cout << endl << "Данному пациенту выдано направление! Удаление невозможно!" << endl << endl;
                return;
            }
            compare_to_delete++;          
        }
    }
    cout << endl;
}

void ClearDataBase_Patients(Patient* spreadsheet_of_patient)
{
    for (int i = 0; i < size_of_spreadsheet; i++)
    {
        spreadsheet_of_patient[i].SetZeroes();
    }
}

void Patient_to_File(Patient* spreadsheet_of_patient)
{
    ofstream fout;
    fout.open(path_to_patient);

    if (!fout.is_open())
    {
        cout << "Ошибка открытия файла" << endl;
        return;
    }

    for (int i = 0; i < size_of_spreadsheet; i++)
    {
        if (spreadsheet_of_patient[i].Year_of_birth != 0)
        {
            fout << spreadsheet_of_patient[i].RegistrationNumber << "\n";
            fout << spreadsheet_of_patient[i].FIO << "\n";
            fout << spreadsheet_of_patient[i].Address << "\n";
            fout << spreadsheet_of_patient[i].Place_of_Work_or_Study << "\n";
            fout << spreadsheet_of_patient[i].Year_of_birth << '\n';
        }
    }
    fout.close();
}

void Patient_from_File(Patient* spreadsheet_of_patient)
{
    ifstream fin;
    fin.open(path_to_patient);

    if (!fin.is_open())
    {
        cout << "Ошибка открытия файла" << endl;
        return;
    }

    Patient tmp;
    char ex;
    int max_id = -1;
    int tmp_id;
    int hashed_card;

    while (!fin.eof())
    {
        tmp.SetZeroes();
        fin >> tmp.RegistrationNumber;
        fin.get(ex);
        getline(fin, tmp.FIO);

        if (tmp.FIO == "\0")
        {
            break;
        }

        getline(fin, tmp.Address);
        getline(fin, tmp.Place_of_Work_or_Study);
        fin >> tmp.Year_of_birth;
        tmp_id = (int(tmp.RegistrationNumber[3]) - 48) * 100000 + (int(tmp.RegistrationNumber[4]) - 48) * 10000 + (int(tmp.RegistrationNumber[5]) - 48) * 1000 + (int(tmp.RegistrationNumber[6]) - 48) * 100 + (int(tmp.RegistrationNumber[7]) - 48) * 10 + (int(tmp.RegistrationNumber[8]) - 48) * 1;

        if (tmp_id > max_id)
        {
            max_id = tmp_id;
        }

        hashed_card = HashFunction(tmp.RegistrationNumber);
        FillHashSpreadsheet(spreadsheet_of_patient, tmp, hashed_card);
    }
    fin.close();
    Patient::id = max_id + 1;

    system("cls");
    return;
}

void Position_check(string& position, string text)
{
    while (true)
    {
        cout << text;
        getline(cin, position);
        bool incorrect = false;

        if (position.length() > 25)
        {
            cout << "Ошибка! Повторите ввод" << endl << endl;
            continue;
        }

        if (position[0] == '.' or position[0] == '-')
        {
            cout << "[" << position[0] << "]" << " - Недопустимый символ!" << endl;
            continue;
        }

        int check = 0;
        for (size_t i = 0; i < position.length(); i++)
        { 
            if (int('А') <= int(position[i]) && int(position[i]) <= int('Я') || int('а') <= int(position[i]) && int(position[i]) <= int('я') ||
                int(position[i]) == int(' ') || int(position[i]) == int('.') || int(position[i]) == int('ё') || int(position[i]) == int('Ё') ||
                int(position[i]) == int('-'))
            {

            }
            else
            {
                cout << position[i] << " - Недопустимый символ!" << endl;
                incorrect = true;
                break;
            }
        }

        if (incorrect)
        {
            continue;
        }

        else
        {
            break;
        }
    }
}

void Admission_Schedule_check(string& admission_schedule, string text)
{
    while (true)
    {
        cout << text;
        getline(cin, admission_schedule);
        bool incorrect = false;

        if (admission_schedule.length() == 1)
        {
            for (size_t i = 0; i < admission_schedule.length(); i++)
            {
                if (int('8') <= int(admission_schedule[i]) && int(admission_schedule[i]) <= int('9'))
                {

                }
                else
                {
                    cout << admission_schedule[i] << " - Недопустимый символ!" << endl;
                    incorrect = true;
                    break;
                }
            }
            if (incorrect)
            {
                continue;
            }
            else
            {
                break;
            }
        }
        else if (admission_schedule.length() == 2)
        {
            for (size_t i = 0; i < admission_schedule.length(); i++)
            {
                if (int('1') == int(admission_schedule[i]))
                {
                    break;
                }

                else
                {
                    cout << admission_schedule[i] << " - Недопустимый символ!" << endl;
                    incorrect = true;
                    break;
                }
            }

            for (size_t i = 1; i < admission_schedule.length(); i++)
            {
                if (int('0') <= int(admission_schedule[i]) && int(admission_schedule[i]) <= int('7'))
                {

                }
                else
                {
                    cout << admission_schedule[i] << " - Недопустимый символ!" << endl;
                    incorrect = true;
                    break;
                }
            }
            if (incorrect)
            {
                continue;
            }
            else
            {
                break;
            }
        }
        else
        {
            cout << " Ошибка ввода!" << endl;                     
        }

    }
}

void InfoAboutDoctor(Doctor& new_doctor)
{
    string  FIO, Position,Admission_Schedule, start_hours, end_hours;
    int Cabinet_Number;

    Name_check(FIO, "Введите ФИО доктора(до 25 символов): ");
    cout << endl;
    Position_check(Position, "Введите должность доктора(до 25 символов): ");
    cout << endl;
    Cabinet_Number = Int_check("Введите номер кабинета доктора(1-24): ");
    cout << endl;

    while (true)
    {
        Admission_Schedule_check(start_hours, "Введите время начала приема (часы): ");
        cout << endl;
        Admission_Schedule_check(end_hours, "Введите время окончания приема (часы): ");

        if (start_hours.length() == 1)
        {
            start_hours = "0" + start_hours;
            break;
        }

        if (end_hours.length() == 1)
        {
            end_hours = "0" + end_hours;
            break;
        }

        if (end_hours.length() == 2)
        {
            break;
        }

        if (start_hours >= end_hours)
        {
            cout << "Ошибка! Введите время приема повторно." << endl << endl;
        }
    }
    cout << endl;

    Admission_Schedule = start_hours + ":00 - " + end_hours + ":00";

    new_doctor.SetInfo(FIO, Position, Admission_Schedule, Cabinet_Number);    
}

void Find_Doctor_Position(Doctor* p, string Position)
{
    if (p != nullptr)
    {
        Find_Doctor_Position(p->left, Position);

        bool found = true;
        if (Position.length() <= p->Position.length())
        {
            for (int i = 0; i < (p->Position.length() - Position.length() + 1); i++)
            {
                found = true;
                int k = i;

                for (int j = 0; j < Position.length(); j++)
                {

                    if (p->Position[k] != Position[j])
                    {
                        found = false;

                        break;
                    }
                    k++;
                }
                if (found)
                {                  
                    cout << "   |" << setw(25) << p->FIO << "|" << setw(25) << p->Position << "|" << setw(7) << p->Cabinet_Number << "|" << setw(14) << p->Admission_Schedule << "|" << endl;
                    break;
                }
            }
        }
        Find_Doctor_Position(p->right, Position);
    }
}

Doctor* Find_Doctor_FIO(Doctor* tmp, string FIO)
{
    while (tmp != nullptr)
    {
        if (tmp->FIO == FIO)
        {
            return tmp;
        }

        if (tmp->FIO > FIO)
        {
            tmp = tmp->left;
        }

        else
        {
            tmp = tmp->right;
        }
    }
    return nullptr;
}

int height(Doctor* p) // высота
{
    return p ? p->height : 0;
}

int bfactor(Doctor* p)  //разность высот левого и правого поддеревьев
{
    return height(p->right) - height(p->left);
}

void fixheight(Doctor* p) // восстановка корректного поля высоты заданного узла
{
    int hl = height(p->left);
    int hr = height(p->right);
    p->height = (hl > hr ? hl : hr) + 1;
}

Doctor* rotate_right(Doctor* p) // правый поворот вокруг p
{
    Doctor* q = p->left;
    p->left = q->right;
    q->right = p;
    fixheight(p);
    fixheight(q);
    return q;
}

Doctor* rotate_left(Doctor* q) // левый поворот вокруг q
{
    Doctor* p = q->right;
    q->right = p->left;
    p->left = q;
    fixheight(q);
    fixheight(p);
    return p;
}

Doctor* balance(Doctor* p) // балансировка
{
    fixheight(p);
    if (bfactor(p) == 2)
    {
        if (bfactor(p->right) < 0)
            p->right = rotate_right(p->right);
        return rotate_left(p);
    }
    if (bfactor(p) == -2)
    {
        if (bfactor(p->left) > 0)
            p->left = rotate_left(p->left);
        return rotate_right(p);
    }
    return p;
}

Doctor* Insert_Doctor(Doctor* p, Doctor& new_doctor) // вставка ключа k в дерево с корнем p
{
    if (!p)
    {
        Doctor* new_elem_for_insert = new Doctor;
        new_elem_for_insert->SetInfo(new_doctor);
        return new_elem_for_insert; 
    }
    if (new_doctor.FIO < p->FIO)
    {
        p->left = Insert_Doctor(p->left, new_doctor);
    }
    else
        p->right = Insert_Doctor(p->right, new_doctor);
    return balance(p);
}

Doctor* find_min(Doctor* p) // поиск узла с минимальным ключом в дереве p 
{
    return p->left ? find_min(p->left) : p;
}

Doctor* Clear_Doctor_FIO_min(Doctor* p) // удаление узла с минимальным ключом из дерева p
{
    if (p->left == 0)
        return p->right;
    p->left = Clear_Doctor_FIO_min(p->left);
    return balance(p);
}

Doctor* Clear_Doctor_FIO(Doctor* p, string FIO, Referral_to_Doctor* start)// удаление ключа k из дерева p
{
    if (!p)
    {
        cout << "Доктор не найден." << endl << endl;
        return nullptr;
    }

    if (FIO < p->FIO)
    {
        p->left = Clear_Doctor_FIO(p->left, FIO, start);
    }

    else if (FIO > p->FIO)
        p->right = Clear_Doctor_FIO(p->right, FIO, start);
    else
    {
        if (Can_delete_Doctor(start, FIO))
        {
            Doctor* q = p->left;
            Doctor* r = p->right;
            delete p;
            cout << "Данные о докторе успешно удалены." << endl << endl;
            if (!r) return q;
            Doctor* min = find_min(r);
            min->right = Clear_Doctor_FIO_min(r);
            min->left = q;
            return balance(min);
        }
        else
        {
            cout << "Удаление невозможно! К доктору имеется направление! " << endl << endl;

        }
    }
    return balance(p);
}

void ClearDataBase_Doctor(Doctor* p)
{   
    if (p != nullptr)
    {
        ClearDataBase_Doctor(p->left);
        ClearDataBase_Doctor(p->right);
        delete p;
    }
}

void Show_Doctors_Tree(Doctor* p, int &number)
{      
    if (p != NULL)
    {
        Show_Doctors_Tree(p->left, number);
        number++;
        cout << setw(3) << number << "|" << setw(25) << p->FIO << "|" << setw(25) << p->Position << "|" << setw(7) << p->Cabinet_Number << "|" << setw(14) << p->Admission_Schedule << "|" << endl;
        Show_Doctors_Tree(p->right, number);
    }
}

int tabs = 0;

void tree_print_beautiful(Doctor* p)
{
    if (p != NULL)
    {
        tabs += 5;
        tree_print_beautiful(p->left);
        for (int i = 0; i < tabs; i++) cout << " ";
       
        cout << p->FIO << endl;

        tree_print_beautiful(p->right);
        tabs -= 5;
    }
}

void Tree_in_File(Doctor* p, ofstream& fout)
{
    if ( p != nullptr)
    {
        Tree_in_File(p->left, fout);
        fout << p->FIO << "\n";
        fout << p->Position << "\n";
        fout << p->Admission_Schedule << "\n";
        fout << p->Cabinet_Number << '\n';
        Tree_in_File(p->right, fout);
    }
}

void Doctors_to_File(Doctor* p)
{
    ofstream fout;
    fout.open(path_to_doctors);    

    if (!fout.is_open())
    {
        cout << "Ошибка открытия файла" << endl;
        return;
    }

    Tree_in_File(p, fout);
    fout.close();
}

void Doctors_from_File(Doctor*& p)
{
    ifstream fin;
    fin.open(path_to_doctors);

    if (!fin.is_open())
    {
        cout << "Ошибка открытия файла" << endl;
        return;
    }

    Doctor new_doctor;    

    while (!fin.eof())
    {
        new_doctor.SetZeroes();
        getline(fin, new_doctor.FIO);

        if (new_doctor.FIO == "\0")
        {
            break;
        }

        getline(fin, new_doctor.Position);
        getline(fin, new_doctor.Admission_Schedule);
        fin >> new_doctor.Cabinet_Number;
        fin.get();
        p = Insert_Doctor(p,new_doctor);
    }    

    fin.close();
    system("cls");
    return;
}

string ChekDate()
{
    int day, month, year;
    string date = "\0";

    while (true)
    {
        cout << "Ввод даты: " << endl;
        day = Int_check("Введите день: ");
        month = Int_check("Введите месяц: ");
        year = Int_check("Введите год: ");

        if (day > 28 and month == 2 and year % 4 != 0)
        {
            cout << "-----------------------------" << endl;
            cout << "| Заданная дата некорректна |" << endl;
            cout << "-----------------------------" << endl;
            continue;
        }

        else if (day > 28 and year % 4 == 0 and year % 100 == 0 and year % 400 != 0)
        {
            cout << "-----------------------------" << endl;
            cout << "| Заданная дата некорректна |" << endl;
            cout << "-----------------------------" << endl;
            continue;
        }

        else if (day > 31 and ((month % 2 == 1 and month <= 7) or (month % 2 == 0 and month > 7)))
        {
            cout << "-----------------------------" << endl;
            cout << "| Заданная дата некорректна |" << endl;
            cout << "-----------------------------" << endl;
            continue;
        }

        else if (day > 30 and ((month % 2 == 0 and month <= 7) or (month % 2 == 1 and month > 7)))
        {
            cout << "-----------------------------" << endl;
            cout << "| Заданная дата некорректна |" << endl;
            cout << "-----------------------------" << endl;
            continue;
        }
        break;
    }

    if (day < 10)
    {
        date += "0";
    }

    date += to_string(day);
    date += ".";

    if (month < 10)
    {
        date += "0";
    }

    date += to_string(month);
    date += ".";
    date += to_string(year);

    return date;
}

void Clear_List(Referral_to_Doctor*& start)
{
    while (start != nullptr)
    {
        Referral_to_Doctor* tmp = start;
        start = start->Next;
        delete tmp;
    }
}

void Layer_List(Referral_to_Doctor*& start, Referral_to_Doctor* writes, int amount_of_writes)
{
    for (int i = 0; i < amount_of_writes; i++)
    {
        if (start == nullptr)
        {
            start = new Referral_to_Doctor(writes[i].RegistrationNumber_Patient, writes[i].FIO_Doctor, writes[i].Referral_time, writes[i].Referral_date);
        }

        else
        {
            Referral_to_Doctor* tmp = start;

            while (tmp->Next != nullptr)
            {
                tmp = tmp->Next;
            }
            tmp->Next = new Referral_to_Doctor(writes[i].RegistrationNumber_Patient, writes[i].FIO_Doctor, writes[i].Referral_time, writes[i].Referral_date);
        }
    }

    Referral_to_Doctor* tmp = start;
    Referral_to_Doctor* tmp_block = start;

    while (tmp->Next != nullptr)
    {
        if (tmp_block->FIO_Doctor[0] != tmp->FIO_Doctor[0])
        {
            tmp_block->Next_block = tmp;
            tmp_block = tmp;
        }
        tmp = tmp->Next;
    }

    if (tmp_block->FIO_Doctor[0] != tmp->FIO_Doctor[0])
    {
        tmp_block->Next_block = tmp;
        tmp_block = tmp;
    }
    tmp = tmp->Next;
}

void Swap(Referral_to_Doctor& first, Referral_to_Doctor& second)
{
    Referral_to_Doctor tmp = first;
    first = second;
    second = tmp;
}

void ShakerSort(Referral_to_Doctor*& start)
{
    Referral_to_Doctor* tmp = start;
    int amount_of_writes = 0;

    while (tmp != nullptr)
    {
        amount_of_writes++;
        tmp = tmp->Next;
    }

    if (amount_of_writes == 0)
    {
        return;
    }

    Referral_to_Doctor* writes = new Referral_to_Doctor[amount_of_writes];
    tmp = start;

    for (int i = 0; i < amount_of_writes; i++)
    {
        writes[i].RegistrationNumber_Patient = tmp->RegistrationNumber_Patient;
        writes[i].FIO_Doctor = tmp->FIO_Doctor;
        writes[i].Referral_time = tmp->Referral_time;
        writes[i].Referral_date = tmp->Referral_date;

        tmp = tmp->Next;
    }
    Clear_List(start);

    // шейкерная сортировка ))
    int leftMark = 1;
    int rightMark = amount_of_writes - 1;

    while (leftMark <= rightMark)
    {
        for (int i = rightMark; i >= leftMark; i--)
        {
            if (writes[i - 1].FIO_Doctor > writes[i].FIO_Doctor)
            {
                swap(writes[i - 1], writes[i]);
            }
        }
        leftMark++;

        for (int i = leftMark; i <= rightMark; i++)
        {
            if (writes[i - 1].FIO_Doctor > writes[i].FIO_Doctor)
            {
                swap(writes[i - 1], writes[i]);
            }
        }
        rightMark--;
    }

    Layer_List(start, writes, amount_of_writes);
}

void Issuing_Referrals_to_the_Patient(Referral_to_Doctor*& Next, Patient* spreadsheet_of_patient, Doctor* p)
{
    string Reg_Number;
    cout << endl << endl << "Выдача направления к доктору" << endl << endl;
   
    Reg_Number_check(Reg_Number, "Введите регистрационный номер пациента: ");
    
    int adress = Find_Patient_RegNumber(spreadsheet_of_patient, Reg_Number);

    if (adress == size_of_spreadsheet)
    {
        cout << endl << "Пациент с веденным регистрационным номером отсутствует. Выдача направления отменена. " << endl;
        return;
    }
    cout << endl;

    string FIO;

    Name_check(FIO, "Введите ФИО доктора(до 25 символов): ");
    Doctor* new_doctor = Find_Doctor_FIO(p, FIO);

    if (new_doctor == nullptr)
    {
        cout << endl << "Доктор с веденным ФИО отсутствует. Выдача направления отменена. " << endl;
        return;
    }

    string time;

    while (true)
    {
        Admission_Schedule_check(time, "Введите время приема: ");
        cout << endl;

        if (time.length() == 1)
        {
            time = "0" + time;
            break;
        }
        break;
    }

    time = time + ":00";

    string date = ChekDate();   

    string start_time, end_time;
    start_time = end_time = "\0";

    for (int i = 0; i < 5; i++)
    {
        start_time += new_doctor->Admission_Schedule[i];
    }

    for (int i = 8; i < 13; i++)
    {
        end_time += new_doctor->Admission_Schedule[i];
    }

    if (time < start_time or time > end_time)
    {
        cout << "Доктор не принимает в веденное время!" << endl;
        return;
    }

    if (Next == nullptr)
    {
        Next = new Referral_to_Doctor(Reg_Number, FIO, time, date);
        return;
    }

    else
    {
        Referral_to_Doctor* tmp = Next;
        while (tmp->Next != nullptr)
        {
            if (tmp->FIO_Doctor == FIO and tmp->Referral_date == date and tmp->Referral_time == time)
            {
                cout << "Данное время занято для записи" << endl;
                return;
            }
            tmp = tmp->Next;
        }

        if (tmp->FIO_Doctor == FIO and tmp->Referral_date == date and tmp->Referral_time == time)
        {
            cout << "Данное время занято для записи" << endl;
            return;
        }

        tmp->Next = new Referral_to_Doctor(Reg_Number, FIO, time, date);
    }
    cout << "Направление выдано" << endl;
}

void Return_Referrals_to_the_Patient(Referral_to_Doctor*& start, Patient* spreadsheet_of_patient, Doctor* p)
{
    string Reg_Number;
    cout << endl << endl << "Возврат направления к доктору" << endl << endl;

    Reg_Number_check(Reg_Number, "Введите регистрационный номер пациента: ");

    int adress = Find_Patient_RegNumber(spreadsheet_of_patient, Reg_Number);

    if (adress == size_of_spreadsheet)
    {
        cout << endl << "Пациент с веденным регистрационным номером отсутствует. Возврат направления отменен. " << endl;
        return;
    }
    cout << endl;

    string FIO;

    Name_check(FIO, "Введите ФИО доктора(до 25 символов): ");
    Doctor* new_doctor = Find_Doctor_FIO(p, FIO);

    if (new_doctor == nullptr)
    {
        cout << endl << "Доктор с веденным ФИО отсутствует. Возврат направления отменен. " << endl;
        return;
    }

    string date = ChekDate();

    Referral_to_Doctor* tmp = start;

    while (tmp != nullptr)
    {
        if (tmp->Next_block != nullptr)
        {
            tmp->Next_block = nullptr;
        }
        tmp = tmp->Next;
    }
    
    if (start == nullptr)
    {
        cout << "Выданные направления в базе данных отсутствуют." << endl << endl;
        return;
    }

    Referral_to_Doctor* first = start;
    Referral_to_Doctor* second = start->Next;    

    if (second == nullptr and first->RegistrationNumber_Patient == Reg_Number and first->FIO_Doctor == FIO and first->Referral_date == date)
    {
        delete start;
        start = nullptr;
        cout << "Направление успешно удалено." << endl << endl;
        return;
    }

    while (second != nullptr)
    {
        if (second->RegistrationNumber_Patient == Reg_Number and second->FIO_Doctor == FIO and second->Referral_date == date)
        {
            first->Next = second->Next;
            delete second;
            cout << "Направление успешно удалено." << endl << endl;
            return;
        }       

        first = first->Next;
        second = second->Next;
    }
    ShakerSort(start);

    cout << "Направление найти не удалось." << endl << endl;
}

void Show_Referrals(Referral_to_Doctor* start)
{
    int number = 0;
    cout << "Список ввыданных направлений: " << endl << endl;

    cout << "  N|Регистрационный номер|              ФИО доктора| Время приема| Дата приема|" << endl;
    cout << "---|---------------------|-------------------------|-------------|------------|" << endl;
    while (start != nullptr)
    {
        number++;
        cout << setw(3) << number << "|" << setw(21) << start->RegistrationNumber_Patient << "|" << setw(25) << start->FIO_Doctor << "|" << setw(13) << start->Referral_time << "|" << setw(12) << start->Referral_date << "|" << endl;
        start = start->Next;
    }
    cout << endl << endl;
}

void Show_Referrals_Patient(Referral_to_Doctor* start, string Reg_Number)
{
    cout << "Доктора к которым пациент получил направления: " << endl << endl;

    int number = 0;
    cout << "  N|              ФИО доктора|" << endl;
    cout << "---|-------------------------|" << endl;

    while (start != nullptr)
    {
        if (start->RegistrationNumber_Patient == Reg_Number)
        {
            number++;
            cout << setw(3) << number << "|" << setw(25) << start->FIO_Doctor << "|"<< endl;            
        }
        start = start->Next;
    }
}

void Show_Referrals_Doctors(Referral_to_Doctor* start, string FIO, Patient* spreadsheet_of_patient)
{
    cout << "Пациенты, направленые к данному доктору: " << endl << endl;
    cout << "N  |Регистрационный номер|             ФИО пациента|" << endl;
    cout << "---|---------------------|-------------------------|" << endl;

    int number = 0;

    while (start != nullptr)
    {
        if (start->FIO_Doctor == FIO)
        {
            number++;
            int adress = Find_Patient_RegNumber(spreadsheet_of_patient, start->RegistrationNumber_Patient);
            cout << setw(3) << number << "|" << setw(21) << start->RegistrationNumber_Patient << "|" << setw(25) << spreadsheet_of_patient[adress].FIO << "|" << endl;
        }

        if (start->FIO_Doctor[0] != FIO[0])
        {
            start = start->Next_block;
        }
        else
        {
            start = start->Next;
        }
    }
}

void Referral_to_Doctor_from_File(Referral_to_Doctor*& start)
{
    ifstream fin;
    fin.open(path_to_referral);

    if (!fin.is_open())
    {
        cout << "Ошибка открытия файла" << endl;
        return;
    }    

    string reg_number, fio_doctor, referral_time, referral_date;

    while (!fin.eof())
    {         
        reg_number = fio_doctor = referral_time = referral_date = "\0";

        fin >> reg_number;
        fin.get();
        getline(fin, fio_doctor);

        if (fio_doctor == "\0")
        {
            break;
        }

        fin >> referral_time;
        fin >> referral_date;

        if (start == nullptr)
        {
            start = new Referral_to_Doctor(reg_number, fio_doctor, referral_time, referral_date);
        }

        else
        {
            Referral_to_Doctor* tmp = start;

            while (tmp->Next != nullptr)
            {                
                tmp = tmp->Next;
            }
            tmp->Next = new Referral_to_Doctor(reg_number, fio_doctor, referral_time, referral_date);
        }
    }  
    fin.close();
    return;
}

void Referral_to_Doctor_to_File(Referral_to_Doctor* start)
{
    ofstream fout;
    fout.open(path_to_referral);

    if (!fout.is_open())
    {
        cout << "Ошибка открытия файла" << endl;
        return;
    }

    while (start != nullptr)
    {       
        fout << start->RegistrationNumber_Patient << "\n";
        fout << start->FIO_Doctor << "\n";
        fout << start->Referral_time << "\n";
        fout << start->Referral_date << "\n";
        start = start->Next;
    }
    fout.close();
}

int main()
{
    system("color 0F");

    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    
    int menu = 100;

    Patient*Database_Patients = new Patient[size_of_spreadsheet];
    Patient_from_File(Database_Patients);
    
    Doctor* Database_Doctors = NULL;
    Doctors_from_File(Database_Doctors);

    Referral_to_Doctor* Next = nullptr;
    Referral_to_Doctor_from_File(Next);

    while (menu != 0)
    {
        Menu();        
        menu = Int_check("Выберете пункт меню: ");

        switch (menu)
        {
        case 1:
        {
            system("cls");

            int amount_of_patient;
            cout << endl;

            amount_of_patient = Int_check("Введите количество новых больных, которых хотите занести в базу данных поликлиники: ");
            cout << endl;

            for (int i = 0; i < amount_of_patient; i++)
            {
                Patient new_patient;
                infoAboutPatient(new_patient);
                int hashed_registrat = HashFunction(new_patient.RegistrationNumber);
                FillHashSpreadsheet(Database_Patients, new_patient, hashed_registrat);
            }

            if (amount_of_patient != 0)
            {
                cout << endl;
                cout << "Данные о новых больных добавлены" << endl << endl;
            }
            system("pause");
            break;
        }
        case 2: 
        {
            system("cls");

            SwowSpreadsheet(Database_Patients);
            system("pause");
            break;
        }
        case 3:
        {
            system("cls");
            SwowSpreadsheet(Database_Patients);
            cout << endl;

            Find_Patient_FIO(Database_Patients);
            system("pause");
            break;
        }
        case 4:
        {
            system("cls");
            SwowSpreadsheet(Database_Patients);
            cout << endl;

            string Reg_Number;
            Reg_Number_check(Reg_Number, "Введите регистрационный номер пациента: ");

            int adress = Find_Patient_RegNumber(Database_Patients, Reg_Number);

            if (adress == size_of_spreadsheet)
            {
                cout << endl << "Пациент с веденным регистрационным номером отсутствует.Поиск по регистрационному номеру отменен." << endl;
                system("pause");
                break;
            }

            cout << "N  |Регистрационный номер|             ФИО пациента|                                             Адрес|                               Место работы(учебы)| Год рождения|" << endl;
            cout << "---|---------------------|-------------------------|--------------------------------------------------|--------------------------------------------------|-------------|" << endl;
            cout << "  1|" << setw(21) << Database_Patients[adress].RegistrationNumber << "|" << setw(25) << Database_Patients[adress].FIO << "|" << setw(50) << Database_Patients[adress].Address << "|" << setw(50) << Database_Patients[adress].Place_of_Work_or_Study<< "|" << setw(13) << Database_Patients[adress].Year_of_birth << "|"<< endl;
            cout << endl;

            ShakerSort(Next);
            Show_Referrals_Patient(Next, Reg_Number);
            system("pause");
            break;
        }
        case 5:
        {
            system("cls");

            SwowSpreadsheet(Database_Patients);
            cout << endl << endl;

            ShakerSort(Next);
            Clear_Patient_FIO(Database_Patients, Next);
            system("pause");
            break;
        }
        case 6:
        {
            ShakerSort(Next);

            if (Next == nullptr)
            {
                ClearDataBase_Patients(Database_Patients);
                cout << "База данных пациентов полностью очищена" << endl << endl;
                system("pause");
            }

            else
            {
                cout << "Базу данных очистить нельзя! Имеются ввыданные направления!" << endl << endl;
            }

            system("pause");
            break;
        }        
        case 7:
        {
            system("cls");

            int amount_of_patient;
            cout << endl;

            amount_of_patient = Int_check("Введите количество новых врачей, которых хотите занести в базу данных поликлиники: ");
            cout << endl;

            for (int i = 0; i < amount_of_patient; i++)
            {
                Doctor new_doctor;
                InfoAboutDoctor(new_doctor);
                Database_Doctors = Insert_Doctor(Database_Doctors, new_doctor);
            }

            if (amount_of_patient != 0)
            {
                cout << endl;
                cout << "Данные о новых врачах добавлены" << endl << endl;
            }

            system("pause");
            break;
        }
        case 8:
        {            
            system("cls");

            int number = 0;
            cout << "'Список зарегестрированных докторов больницы'" << endl << endl;
            cout << "   |              ФИО доктора|                Должность| N каб.| График приема|" << endl;
            cout << "---|-------------------------|-------------------------|-------|--------------|" << endl;
            Show_Doctors_Tree(Database_Doctors, number);
            cout << endl << endl;

            tree_print_beautiful(Database_Doctors);
            cout << endl;
            system("pause");
            break;
        }
        case 9:
        {
            system("cls");

            int number = 0;

            cout << "'Список зарегестрированных докторов больницы'" << endl << endl;
            cout << "  N|              ФИО доктора|                Должность| N каб.| График приема|" << endl;
            cout << "---|-------------------------|-------------------------|-------|--------------|" << endl;
            Show_Doctors_Tree(Database_Doctors, number);
            cout << endl << endl;

            string FIO;

            Name_check(FIO, "Введите ФИО доктора(до 25 символов): ");
            Doctor* new_doctor = Find_Doctor_FIO(Database_Doctors, FIO);

            if (new_doctor == nullptr)
            {
                cout << endl << "Доктор с веденным ФИО отсутствует. Поиск по ФИО отменен." << endl;
                system("pause");
                break;
            }

            cout << endl << "Найденный доктор: " << endl;
            cout << "  N|              ФИО доктора|                Должность| N каб.| График приема|" << endl;
            cout << "---|-------------------------|-------------------------|-------|--------------|" << endl;
            cout << "  1|" << setw(25) << new_doctor->FIO << "|" << setw(25) << new_doctor->Position << "|" << setw(7) << new_doctor->Cabinet_Number << "|" << setw(14) << new_doctor->Admission_Schedule << "|" << endl;
            cout << endl << endl;

            ShakerSort(Next);

            Show_Referrals_Doctors(Next, FIO, Database_Patients);
            cout << endl << endl;
            system("pause");
            break;
        }
        case 10:
        {
           system("cls");

            int number = 0;
            cout << "'Список зарегестрированных докторов больницы'" << endl << endl;
            cout << "   |              ФИО доктора|                Должность| N каб.| График приема|" << endl;
            cout << "---|-------------------------|-------------------------|-------|--------------|" << endl;
            Show_Doctors_Tree(Database_Doctors, number);

            cout << endl << endl << "'Поиск доктора по фрагменту Должность'" << endl << endl;

            string Position;
            Position_check(Position, "Введите должность доктора(до 25 символов): ");
            cout << endl;

            cout << "Найденный доктор: " << endl << endl;
            cout << "   |              ФИО доктора|                Должность| N каб.| График приема|" << endl;
            cout << "---|-------------------------|-------------------------|-------|--------------|" << endl;
            Find_Doctor_Position(Database_Doctors, Position);

            cout << endl;
            system("pause");
            break;
        }
        case 11:
        {
            system("cls");

            int number = 0;
            cout << "'Список зарегестрированных докторов больницы'" << endl << endl;
            cout << "   |              ФИО доктора|                Должность| N каб.| График приема|" << endl;
            cout << "---|-------------------------|-------------------------|-------|--------------|" << endl;

            Show_Doctors_Tree(Database_Doctors, number);

            cout << endl << endl << "'Удаление доктора по ФИО'" << endl << endl;

            string FIO;
            Name_check(FIO, "Введите ФИО доктора(до 25 символов): ");
            cout << endl; 

            ShakerSort(Next);
            Database_Doctors = Clear_Doctor_FIO(Database_Doctors, FIO, Next);
            
            system("pause");
            break;
        }
        case 12:
        {
            ShakerSort(Next);

            if (Next == nullptr)
            {
                ClearDataBase_Doctor(Database_Doctors);

                Database_Doctors = nullptr;

                cout << "База данных врачей полностью очищена" << endl << endl;
                system("pause");
            }

            else
            {
                cout << "Базу данных очистить нельзя! Имеются ввыданные направления!" << endl << endl;
            }
            system("pause");
            break;
        }
        case 13:
        {
            system("cls");
            SwowSpreadsheet(Database_Patients);
            cout << endl;

            int number = 0;
            cout << "'Список зарегестрированных докторов больницы'" << endl << endl;
            cout << "   |              ФИО доктора|                Должность| N каб.| График приема|" << endl;
            cout << "---|-------------------------|-------------------------|-------|--------------|" << endl;
            Show_Doctors_Tree(Database_Doctors, number);
            
            Issuing_Referrals_to_the_Patient(Next,Database_Patients,Database_Doctors);
            system("pause");            
            break;
        }
        case 14:
        {
            system("cls");

            Show_Referrals(Next);
            system("pause");
            break;
        }
        case 15:
        {
            system("cls");

            ShakerSort(Next);
            Show_Referrals(Next);
            Return_Referrals_to_the_Patient(Next, Database_Patients, Database_Doctors);
            system("pause");
            break;
        }
        default:            
            break;
        }
    }

    Patient_to_File(Database_Patients);
    delete[]Database_Patients;

    Doctors_to_File(Database_Doctors);
    ClearDataBase_Doctor(Database_Doctors);

    Referral_to_Doctor_to_File(Next);

    system("pause");
    return 0;
}