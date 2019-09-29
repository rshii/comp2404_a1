#include "itunes_interface.hpp"
#include <iostream>
#include <fstream>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::ofstream;

vector<string> ItunesInterface::parseCommands(string arg) {
    int indexOfInterest = 0;
    vector<string> v;

    ItunesInterface::appendCommandLog(arg);

    for (int i = 0; i < arg.length(); i++) {
        // quote to quote traversal
        if (arg[i] == '"') {
            indexOfInterest = i;
            for (int j = i; j < arg.length(); j++) {
                if (arg[j] == '"' && j!=i) {
                    indexOfInterest = j;
                    break;
                }
            }
            if (indexOfInterest == i) {
                cout << ERROR_OPEN_DOUBLE_QUOTE << endl;
                ItunesInterface::appendOutputLog(ERROR_OPEN_DOUBLE_QUOTE);
                return vector<string> ();
            }
            else if (indexOfInterest == i + 1) {
                cout << ERROR_EMPTY_DOUBLE_QUOTE << endl;
                ItunesInterface::appendOutputLog(ERROR_EMPTY_DOUBLE_QUOTE);
                return vector<string> ();
            }
            else {
                v.push_back(arg.substr(i+1, indexOfInterest - i - 1));
                i = indexOfInterest;
            }
        }
        // ignoring of spaces outsides quotes traversal
        else if (arg[i] == ' ') {
            for (int j = i; j < arg.length(); j++) {
                if (arg[j] == ' ') {
                    indexOfInterest = j;
                }
                else {
                    break;
                }
            }
            i = indexOfInterest;
        }
        // unquoted unspaced character traversal
        else {
            for (int j = i; j < arg.length(); j++) {
                if (arg[j] == ' ' || arg[j] == '"') {
                    break;
                }
                //  enables parsing of comments
                else if (arg[j] == '/' && j < arg.length() - 1 && arg[j+1] == '/') {
                        if (i != j) {
                            v.push_back(arg.substr(i,j-i));
                        }
                        v.push_back(arg.substr(j,arg.length() - j));
                        return v;
                }
                else {
                    indexOfInterest = j;
                }
            }
            v.push_back(arg.substr(i, indexOfInterest - i + 1));
            i = indexOfInterest;
        }
    }

    return v;
}

void ItunesInterface::executeCommands(vector<string> v) {
    if (v.size() == 0) {
        cout << "ERROR: Nothing to execute" << endl;
        return;
    }

    // extract and log, only used for cmdline comments, his instructions are unclear and this can be a case
    // will be processed as a command output
    if (v.back().length() > 1 && v.back().substr(0,2) == "//") {
        if (readingMode == ENUM_READ::IDLE) {
            ItunesInterface::appendOutputLog(v.back());
        }
        v.pop_back();
    }

    if (v.size() == 0) {
        return;
    }
    string cmd = v[0];
    if (cmd == "add") {
        ItunesInterface::add(v);
    }
    else if (cmd == "show") {
        ItunesInterface::show(v);
    }
    else if (cmd == "delete") {
        ItunesInterface::del(v);
    }
    else if (cmd == ".help") {
        ItunesInterface::dot_help();
    }
    else if (cmd == ".read") {
        ItunesInterface::dot_read(v);
    }
    else if (cmd == ".log") {
        ItunesInterface::dot_log(v);
    }
    else if (cmd == ".trim") {
        ItunesInterface::dot_trim(v);
    }
    else if (cmd == ".startsWith") {
        ItunesInterface::dot_startsWith(v);
    }
    else if (cmd == ".endsWith") {
        ItunesInterface::dot_endsWith(v);
    }
    else if (cmd == ".toTitleCase"){
        ItunesInterface::dot_toTitleCase(v);
    }
    else {
        cout << "ERROR: Invalid command: " << cmd << endl;
    }
}

void ItunesInterface::add(vector<string> args) {
    cout << "PARSED COMMAND:" << endl;
    if (args.size() < 2) {
        cout << "ERROR: more arguments required for add cmd" << endl;
        return;
    }
    for (vector<string>::iterator it = args.begin(); it != args.end(); it++) {
        cout << *it << endl;
    }
}

void ItunesInterface::show(vector<string> args) {
    cout << "PARSED COMMAND:" << endl;
    if (args.size() < 2) {
        cout << "ERROR: more arguments required for show cmd" << endl;
        return;
    }
    for (vector<string>::iterator it = args.begin(); it != args.end(); it++) {
        cout << *it << endl;
    }
}

void ItunesInterface::del(vector<string> args) {
    cout << "PARSED COMMAND:" << endl;
    if (args.size() < 2) {
        cout << "ERROR: more arguments required for delete cmd" << endl;
        return;
    }
    for (vector<string>::iterator it = args.begin(); it != args.end(); it++) {
        cout << *it << endl;
    }
}

void ItunesInterface::dot_help() {
    cout<<"================================================="<<endl;
    cout<<"================================================="<<endl;
    cout<<"MyTunes music library"<<endl;
    cout<<"Command Language Version 0.1 (Alpha) Sept 2019"<<endl;
    cout<<"================================================="<<endl;
    cout<<"COMMANDS:"<<endl;
    cout<<""<<endl;
    cout<<"add //add data to collections"<<endl;
    cout<<"    //add recordings"<<endl;
    cout<<"    add -r recording_id title artist producer year"<<endl;
    cout<<"    add -r 103 \"Beatles For Sale\" \"Beatles\" \"George Martin\" 1964"<<endl;
    cout<<"    //add songs"<<endl;
    cout<<"    add -s song_id title composer //add song"<<endl;
    cout<<"	add -s 1001 \"Misery\" \"Paul McCartney, John Lennon\""<<endl;
    cout<<"    //add tracks"<<endl;
    cout<<"    add -t album_id songID track_number"<<endl;
    cout<<"    add -t 100 1000 1"<<endl;
    cout<<"    //add users"<<endl;
    cout<<"    add -u user_id name"<<endl;
    cout<<"    add -u ajones \"Anne Jones\""<<endl;
    cout<<"    //add playlists"<<endl;
    cout<<"    add -p user_id, playlist_name"<<endl;
    cout<<"    add -p ajones \"Driving Songs\""<<endl;
    cout<<"    //add playlist tracks"<<endl;
    cout<<"    add -l user_id playlist_name song_id"<<endl;
    cout<<"    add -l ajones \"Driving Songs\" 1001 "<<endl;
    cout<<"   "<<endl;
    cout<<"delete  //delete data from collections"<<endl;
    cout<<"    delete -s song_id //delete song"<<endl;
    cout<<"    delete -s song_id -p playlist_name -u user_name //delete song from playlist"<<endl;
    cout<<"    delete -Global -s song_id -p playlist_name -u user_name"<<endl;
    cout<<"    delete -r recording_id //delete recording "<<endl;
    cout<<"    delete -u user_id //delete user"<<endl;
    cout<<"    delete -p playist_name -u user_id //delete user playlist"<<endl;
    cout<<"    delete -t song_id recording_id //delete track"<<endl;
    cout<<""<<endl;
    cout<<"show    //show entries in collections"<<endl;
    cout<<"    show songs"<<endl;
    cout<<"    show users"<<endl;
    cout<<"    show playlists -u user_id"<<endl;
    cout<<"    show songs -u user_id -p playlist_name"<<endl;
    cout<<"    show recordings"<<endl;
    cout<<"    show tracks"<<endl;
    cout<<""<<endl;
    cout<<"UI SHELL COMMANDS (start with a dot):"<<endl;
    cout<<""<<endl;
    cout<<".quit   //quit the application"<<endl;
    cout<<""<<endl;
    cout<<".read   //read a script file"<<endl;
    cout<<"	.read filename"<<endl;
    cout<<""<<endl;
    cout<<".log    //log commands and output"<<endl;
    cout<<"        //THIS DOT COMMAND IS SCRIPTABLE"<<endl;
    cout<<"	.log clear //clear the logs"<<endl;
    cout<<"	.log start //begin logging commands only"<<endl;
    cout<<"	.log start_output //begin logging output only"<<endl;
    cout<<"	.log start_both  //begin logging commands and output"<<endl;
    cout<<"	.log stop //stop logging"<<endl;
    cout<<"	.log save filename //save log to filename"<<endl;
    cout<<"        .log show //display current log contents on console"<<endl;
    cout<<""<<endl;
    cout<<".help   //show this help menu"<<endl;
    cout<<""<<endl;
    cout<<"//   //comments"<<endl;
    cout<<"     Any input starting with \"//\" or any output"<<endl;
    cout<<"     starting with \"//\" is treated as a comment."<<endl;
    cout<<"     If input on CLI the comment will be written to log file."<<endl;
    cout<<"     Comments may appear in command script file."<<endl;
    cout<<""<<endl;
    cout<<"DEVELOPER COMMANDS (NOT FOR PRODUCTION):"<<endl;
    cout<<"//These provide are to test certain developer functions"<<endl;
    cout<<".trim    //trim the argument"<<endl;
    cout<<"	.trim \"  a   day ago\" //a day ago"<<endl;
    cout<<".startsWith    //test string prefix"<<endl;
    cout<<"	.startsWith \"Hello\" \"Hel\" //true"<<endl;
    cout<<".endsWith    //test string suffix"<<endl;
    cout<<"	.endsWith \"Hello\" \"llo\" //true"<<endl;
    cout<<".toTitleCase    //returns title cased string"<<endl;
    cout<<"	.toTitleCase \"the girl from ipanema\" //Girl From Ipanema, The"<<endl;
    cout<<"=========================================================="<<endl;
    cout<<"=========================================================="<<endl;
}

void ItunesInterface::dot_read(vector<string> args) {
}

void ItunesInterface::dot_log(vector<string> args) {
    if (args.size() < 2) {
        return;
    }
    
    string option = args[1];
    if (option == "clear") {
        loggingContainerVec.clear();
    }
    else if (option == "start") {
        loggingMode = ENUM_LOG::COMMAND_ONLY;
    }
    else if (option == "start_output") {
        loggingMode = ENUM_LOG::OUTPUT_ONLY;
    }
    else if (option == "start_both") {
        loggingMode = ENUM_LOG::FULL;
    }
    else if (option == "stop") {
        loggingMode = ENUM_LOG::OFF;
    }
    else if (option == "save") {
        if (args.size() < 3) {
            cout << "ERROR: No filename specified" << endl;
            return;
        }
        ofstream outfile;
        outfile.open(args[2]);
        for (vector<string>::iterator it = loggingContainerVec.begin(); it != loggingContainerVec.end(); it++) {
            outfile << *it << endl;
        }
        outfile.close();
    }
    else if (option == "show") {
        for (vector<string>::iterator it = loggingContainerVec.begin(); it != loggingContainerVec.end(); it++) {
            cout << *it << endl;
        }
    }
    else {
        cout << "ERROR: Invalid option for .log" << endl;
    }
}

void ItunesInterface::dot_trim(vector<string> args) {

}

void ItunesInterface::dot_startsWith(vector<string> args) {

}

void ItunesInterface::dot_endsWith(vector<string> args) {

}

void ItunesInterface::dot_toTitleCase(vector<string> args) {

}

void ItunesInterface::appendCommandLog(string arg) {
    if (loggingMode == ENUM_LOG::COMMAND_ONLY || loggingMode == ENUM_LOG::FULL) {
        loggingContainerVec.push_back(arg);
    }
}

void ItunesInterface::appendOutputLog(string arg) {
    if (loggingMode == ENUM_LOG::OUTPUT_ONLY || loggingMode == ENUM_LOG::FULL) {
            loggingContainerVec.push_back(arg);
    }
}