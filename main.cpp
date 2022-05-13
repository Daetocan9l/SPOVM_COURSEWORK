#include <iostream>
#include <windows.h>
#include <fileapi.h>
#include <string>
#include <vector>
#include <stdio.h>

//Класс, который содержит информацию о носителе
class CarrierInfo {
private:
    std::string m_name;
    std::string m_filesystem;
    int m_serialNumber;
public:
    void set_name(std::string name){
        m_name = name;
    }
    void set_filesystem(std::string filesystem){
        m_filesystem = filesystem;
    }
    void set_serialNumber(int serialNumber){
        m_serialNumber = serialNumber;
    }
    void showCarrierInfo() {
        std::cout << "Name:" <<  m_name << std::endl;
        std::cout <<  "Filesystem:" << m_filesystem << std::endl;
        std::cout << "SerialNumber:" << m_serialNumber << std::endl;
    }
};

//Класс добавления носителей в вектор
class ScanСarriers {
private:
    std::vector<std::string> m_carriers;
public:
   void add_carriers(){
       char disks[MAX_PATH];
       char *pNext = disks;
       GetLogicalDriveStringsA(sizeof(disks), disks);
       while(*pNext){
           if(GetDriveTypeA(pNext)==DRIVE_REMOVABLE) m_carriers.push_back((std::string)pNext);
           pNext+=strlen(pNext)+1;
       }
       parsingName();
   }
   std::vector<std::string> get_carriers(){
       return m_carriers;
   }
   void parsingName() {
       if(!m_carriers.empty()) {
            for(int i = 0; i < m_carriers.size(); i++) {
                m_carriers[i].erase(m_carriers[i].begin()+2,m_carriers[i].end());
            }
       }
   }
};

//Класс форматирования
class FormattingExFAT {
private:
    std::string m_drive;
public:
    void set_device_name(std::string drive) {
        m_drive = drive;
    }
    void formatting() {
        puts("\n");
        std::string formattool = "format "+m_drive+" /fs:exfat /q";
        system(formattool.c_str());
    }
};

//Функция проверки на ввод
int inputNumber(int min,int max) {
    int number;
    while(1){
        try {
            std::cin>>number;
            if(std::cin.fail())throw("Enter number:");
            if(number<min || number > max) throw("Try again:");
            else break;
        }
        catch(const char *ex) {
            std::cout<<ex;
            std::cin.clear();
            rewind(stdin);
        }
    }
    return number;
}

void showMenu() {
    puts("");
    puts("Choice an operation:");
    puts("1.Format media to file system exFAT");
    puts("2.Show media information");
    puts("0.Exit");
    std::cout<<"Enter number choice:";
}

void showManual(){
    std::cout<<std::endl;
    puts("Utility ALEXANDRIA intended for:");
    puts("1.Formatting the media file system in exFAT.");
    puts("2.Viewing brief information about the carrier");
    puts("Argument info designed to call a hint.");
    puts("Enjoy using ;)");
}

void choice1(FormattingExFAT tool,std::vector<std::string> carriers,int media){
    std::string carrier = carriers[media-1];
    tool.set_device_name(carrier);
    tool.formatting();
}
void choice2(std::vector<std::string> carriers,int media){
    CarrierInfo carrierInfo;
    std::string carrier = carriers[media-1];
    char NameBuffer[MAX_PATH];
    char SysNameBuffer[MAX_PATH];
    DWORD VSNumber;
    DWORD MCLength;
    DWORD FileSF;
    if (GetVolumeInformation(carrier.c_str(),NameBuffer, sizeof(NameBuffer),
                             &VSNumber,&MCLength,&FileSF,SysNameBuffer,sizeof(SysNameBuffer)))
    {
        carrierInfo.set_name(NameBuffer);
        carrierInfo.set_filesystem(SysNameBuffer);
        carrierInfo.set_serialNumber(VSNumber);
    }
   /* PULARGE_INTEGER pam;
    PULARGE_INTEGER allpam;
    if(GetDiskFreeSpaceExW((LPCWSTR)carrier.c_str(),&pam,&allpam,NULL)==NULL) {
    }*/
    puts(carrier.c_str());
    carrierInfo.showCarrierInfo();
    system("pause");
    system("cls");
}

void checkArguments(int argc,char **argv){
    if(argc >2) {
        puts("'ALEXANDRIA info' - utility instruction");
        exit(0);
    }
    if(argc == 2) {
        if(strcmp(argv[1],"info")==0) {
            showManual();
        }
        else {
            puts("'ALEXANDRIA info' - utility instruction");
        }
        exit(0);
    }
}

int main(int argc,char **argv) {
    checkArguments(argc,argv);
    std::vector<std::string> carriersList;
    ScanСarriers scanCarriers;
    FormattingExFAT formattingtool;
    int choiceMedia = 0;
    int choiceMenu = 0;
    scanCarriers.add_carriers();
    carriersList = scanCarriers.get_carriers();
    if(carriersList.empty()) {
        puts("Media not connected");
        system("pause");
        exit(0);
    }
    while(1) {
        std::cout<<"Choice media:"<<std::endl;
        for(int i=0; i<carriersList.size();i++){
            std::cout<<i+1<<"."<<carriersList[i]<<std::endl;
        }
        std::cout<<"Media:";
        choiceMedia = inputNumber(1,carriersList.size());
        showMenu();
        choiceMenu = inputNumber(0,2);
        switch(choiceMenu){
            case 1 : {
                choice1(formattingtool,carriersList,choiceMedia);
                system("pause");
                exit(0);
                break;
            }
            case 2 : {
                choice2(carriersList,choiceMedia);
                break;
            }
            default: {
                exit(0);
            }

        }
    }
    return 0;
}
