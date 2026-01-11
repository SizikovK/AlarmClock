#include <windows.h>  //калл
#include <shellapi.h> //калл
#include <iostream>
#include <thread>
#include <chrono>
#include <filesystem>

using namespace std;

void play_sound(const wchar_t* path) {
    HINSTANCE r = ShellExecuteW(nullptr, L"open", path, nullptr, nullptr, SW_SHOWNORMAL);
}

void set_ring(wchar_t*& path, const wchar_t* sound) {
    int choice = 0;
    while(choice != 3) {
        system("cls");
        
        wcout << "Menu " << sound << ":" << endl;
        cout << "1. Play sound" << endl;
        cout << "2. Set ringtone" << endl;
        cout << "3. Back" << endl;
        cout << "Enter: ";

        cin >> choice;
        switch (choice) {
        case 1:
            play_sound(sound);
            break;
        case 2:
            cout << endl;
            path = const_cast<wchar_t*>(sound);
            wcout << sound << " has been set as a ringtone" << endl;

            system("pause");
            
            choice = 3;
            break;
        case 3:
            break;
        default:
            cout << "Wrong choice" << endl;
            break;
        }
    }
}

string local_time() {
    time_t now = time(nullptr);
    tm* lt = localtime(&now);
    ostringstream oss;
    oss << put_time(lt, "%H:%M");
    return oss.str();
}

int to_minutes(const string& time) {
    int hours = stoi(time.substr(0, 2));
    int minutes = stoi(time.substr(3, 2));
    return hours * 60 + minutes;
}

bool should_ring(const string& now, const string& target, bool& tomorrow) {
    int nmin = to_minutes(now);
    int tmin = to_minutes(target);

    if(nmin > tmin && !tomorrow) {
        tomorrow = true;
    }

    if(!tomorrow) return nmin >= tmin;

    return (nmin <= tmin) && (nmin < 12 * 60);
}

int main() {
    wchar_t* path = nullptr;
    wchar_t* warn = L"warn.mp3";
    wchar_t* clock = L"clock.mp3";
    wchar_t* ship = L"ship.mp3";

    int choice = 0;
    while(choice != 5) {
        system("cls");
        
        cout << "Menu: " << endl;
        cout << "1. Start alarm clock " << endl;
        cout << "2. Sound: warn.mp3" << endl;
        cout << "3. Sound: clock.mp3" << endl;
        cout << "4. Sound: ship.mp3" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter: ";
    
        cin >> choice;

        switch (choice) {
        case 1: {
            if (!path) { 
                cout << "Choose the ringtone\n"; 
                system("pause");
                break; 
            }

            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            string target_time;
            cout << endl << "Enter time (HH:MM): ";
            getline(cin, target_time);

            string message;
            cout << "Enter message: ";
            getline(cin, message);

            system("cls");
            cout << endl << "Wait!" << endl;

            bool tomorrow = false;
            while(true) {
                string now = local_time();
                
                if(should_ring(now, target_time, tomorrow)) {
                    system("cls");
                    cout << "TIME: " << target_time << endl;
                    cout << message << endl;

                    int i = 0;
                    while(i != 6) {
                        play_sound(path);
                        this_thread::sleep_for(chrono::seconds(3));
                        i++;
                    }
                }
                this_thread::sleep_for(chrono::seconds(60));
            }
            break;
        }
        case 2:
            set_ring(path, warn);
            break;

        case 3:
            set_ring(path, clock);
            break;

        case 4:
            set_ring(path, ship);
            break;
        case 5:
            cout << "Bye Bye sugar" << endl;
            break;

        default:
            cout << "Wrong choice" << endl;
            break;
        }
    }
}