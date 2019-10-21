#include "itunes_interface.hpp"
#include <iostream>
#include <fstream>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;

vector<string> ItunesInterface::parseCommands(string arg) {
    vector<string> v;
    int indexOfInterest = 0;

    // truncates inline comment and includes in command log if not scripted
    int commentPos = arg.find("//");
    if (commentPos != -1) {
        if (commentPos != 0) {
            ItunesInterface::appendCommandLog(arg.substr(0, commentPos));
        }
        if (readingMode == ENUM_READ::IDLE) {
            ItunesInterface::appendCommandLog(arg.substr(commentPos, arg.length() - commentPos));
        }
        arg = arg.substr(0, commentPos);
    }
    else {
        ItunesInterface::appendCommandLog(arg);
    }

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
                ItunesInterface::appendOutputLog("ERROR: Unenclosed double quotes");
                return vector<string> ();
            }
            else if (indexOfInterest == i + 1) {
                ItunesInterface::appendOutputLog("ERROR: Empty double quoted argument");
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
    else if (cmd == ".help" && readingMode == ENUM_READ::IDLE) {
        ItunesInterface::dot_help();
    }
    else if (cmd == ".read" && readingMode == ENUM_READ::IDLE) {
        ItunesInterface::dot_read(v);
    }
    else if (cmd == ".log") {
        ItunesInterface::dot_log(v);
    }
    else if (cmd == ".trim" && readingMode == ENUM_READ::IDLE) {
        ItunesInterface::dot_trim(v);
    }
    else if (cmd == ".startsWith" && readingMode == ENUM_READ::IDLE) {
        ItunesInterface::dot_startsWith(v);
    }
    else if (cmd == ".endsWith" && readingMode == ENUM_READ::IDLE) {
        ItunesInterface::dot_endsWith(v);
    }
    else if (cmd == ".toTitleCase" && readingMode == ENUM_READ::IDLE){
        ItunesInterface::dot_toTitleCase(v);
    }
    else {
        ItunesInterface::appendOutputLog("ERROR: Invalid command: " + cmd);
    }
}

void ItunesInterface::add(vector<string> args) {
    ItunesInterface::appendOutputLog("PARSED COMMAND:");
    if (args.size() < 2) {
        ItunesInterface::appendOutputLog("ERROR: more arguments required for add cmd");
        return;
    }
    for (vector<string>::iterator it = args.begin(); it != args.end(); it++) {
        ItunesInterface::appendOutputLog(*it);
    }
}

void ItunesInterface::show(vector<string> args) {
    ItunesInterface::appendOutputLog("PARSED COMMAND:");
    if (args.size() < 2) {
        ItunesInterface::appendOutputLog("ERROR: more arguments required for show cmd");
        return;
    }
    for (vector<string>::iterator it = args.begin(); it != args.end(); it++) {
        ItunesInterface::appendOutputLog(*it);
    }
}

void ItunesInterface::del(vector<string> args) {
    ItunesInterface::appendOutputLog("PARSED COMMAND:");
    if (args.size() < 2) {
        ItunesInterface::appendOutputLog("ERROR: more arguments required for delete cmd");
        return;
    }
    for (vector<string>::iterator it = args.begin(); it != args.end(); it++) {
        ItunesInterface::appendOutputLog(*it);
    }
}

void ItunesInterface::dot_help() {
    cout<<"================================================="<<endl;
    cout<<"================================================="<<endl;
    cout<<"MyTunes music library"<<endl;
    cout<<"Command Language Version 0.1 (Alpha) Sept 2019"<<endl;
    cout<<"Rev 1: 2019 10 08: change some song_id to track_id to reflect "<<endl;
    cout<<"that playlists refer to tracks not songs i.e. title,composer etc."<<endl;
    cout<<"Also modified the language to be consistent with the"<<endl;
    cout<<"insert_beatles_tracks_rev2.txt insert script"<<endl;
    cout<<""<<endl;
    cout<<"Rev 2: 2019 10 09"<<endl;
    cout<<"For consistency the delete commands have a -t version but the -s version still there"<<endl;
    cout<<"but annotated as DEPRECATED which means you should still implement it."<<endl;
    cout<<""<<endl;
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
    cout<<"    add -t track_id album_id song_id track_number"<<endl;
    cout<<"    add -t 10 100 1000 1"<<endl;
    cout<<"    //add users"<<endl;
    cout<<"    add -u user_id name"<<endl;
    cout<<"    add -u ajones \"Anne Jones\""<<endl;
    cout<<"    //add playlists"<<endl;
    cout<<"    add -p user_id, playlist_name"<<endl;
    cout<<"    add -p ajones \"Driving Songs\""<<endl;
    cout<<"    //add playlist tracks"<<endl;
    cout<<"    add -l user_id playlist_name track_id"<<endl;
    cout<<"    add -l ajones \"Driving Songs\" 1001 "<<endl;
    cout<<"   "<<endl;
    cout<<"delete  //delete data from collections"<<endl;
    cout<<"    delete -s song_id //delete song based on song id (based on title, composer etc.)"<<endl;
    cout<<"    delete -s track_id -p playlist_name -u user_name //DEPRECATED delete track from playlist"<<endl;
    cout<<"    delete -t track_id -p playlist_name -u user_name //delete track from playlist"<<endl;
    cout<<"    delete -Global -s track_id -p playlist_name -u user_name  //DEPRECATED but still supported"<<endl;
    cout<<"    delete -Global -t track_id -p playlist_name -u user_name"<<endl;
    cout<<"    delete -r recording_id //delete recording "<<endl;
    cout<<"    delete -u user_id //delete user"<<endl;
    cout<<"    delete -p playist_name -u user_id //delete user playlist"<<endl;
    cout<<"    delete -t track_id  //delete track"<<endl;
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
    cout<<"	.trim   a   day ago //a day ago"<<endl;
    cout<<".startsWith    //test string prefix"<<endl;
    cout<<"	.startsWith Hello Hel //true"<<endl;
    cout<<".endsWith    //test string suffix"<<endl;
    cout<<"	.endsWith Hello llo //true"<<endl;
    cout<<".toTitleCase    //returns title cased string"<<endl;
    cout<<"	.toTitleCase the girl from ipanema //Girl From Ipanema, The"<<endl;
    cout<<"=========================================================="<<endl;
    cout<<"=========================================================="<<endl;
}

void ItunesInterface::dot_read(vector<string> args) {
    if (args.size() < 2) {
        ItunesInterface::appendOutputLog("ERROR: no filename specified");
        return;
    }

    ifstream infile (args[1]);
    if (!infile) {
        ItunesInterface::appendOutputLog("ERROR: could not open file");
        return;
    }
    string input;
    readingMode = ENUM_READ::READING;
    while(getline(infile, input)) {
        ItunesInterface::executeCommands(
            ItunesInterface::parseCommands(input)
        );
    }
    readingMode = ENUM_READ::IDLE;
    infile.close();
}

void ItunesInterface::dot_log(vector<string> args) {
    if (args.size() < 2) {
        cout << "ERROR: Not enough arguments for .log" << endl;
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
    else if (option == "save" && readingMode == ENUM_READ::IDLE) {
        if (args.size() < 3) {
            ItunesInterface::appendOutputLog("ERROR: No filename specified");
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
        cout << "============="<< endl;
        cout << "     LOG     "<< endl;
        cout << "============="<< endl;
        for (vector<string>::iterator it = loggingContainerVec.begin(); it != loggingContainerVec.end(); it++) {
            cout << *it << endl;
        }
    }
    else {
        ItunesInterface::appendOutputLog("ERROR: Invalid option for .log");
    }
}

void ItunesInterface::appendCommandLog(string arg) {
    if (loggingMode == ENUM_LOG::COMMAND_ONLY || loggingMode == ENUM_LOG::FULL) {
        loggingContainerVec.push_back(arg);
    }
}

void ItunesInterface::appendOutputLog(string arg) {
    cout << arg << endl;
    if (loggingMode == ENUM_LOG::OUTPUT_ONLY || loggingMode == ENUM_LOG::FULL) {
            loggingContainerVec.push_back(arg);
    }
}

void ItunesInterface::dot_trim(vector<string> args) {
    // everything was trimmed in the earlier parsed command,
    // these functions must be accessed through execute parsed command by design
    if (args.size() < 2) {
        ItunesInterface::appendOutputLog("ERROR: .trim needs one argument");
        return;
    }
    string trimmedBuilder = "";
    cout << "ALERT: Spaces in double quoted strings are not trimmed, use unquoted arguments to trim" << endl;
    for (int i = 1; i < args.size(); i++) {
        trimmedBuilder += args[i];
        if (i < args.size() - 1) {
            trimmedBuilder += " ";
        }
    }
        ItunesInterface::appendOutputLog("Trimmed output: " + trimmedBuilder);
}

void ItunesInterface::dot_startsWith(vector<string> args) {
    if (args.size() < 3) {
        ItunesInterface::appendOutputLog("ERROR: .startsWith needs at least two arguments");
        return;
    }
    cout << "ALERT: spacing in double quoted arguments will not be trimmed" << endl;
    string root = args[1];
    string prefix = args[2];
    if (prefix.length() > root.length()) {
        ItunesInterface::appendOutputLog("ERROR: .startsWith prefix longer than root");
        return;
    }
    if (root.substr(0,prefix.length()) == prefix) {
        ItunesInterface::appendOutputLog("StartsWith output: " + prefix + " is a prefix to " + root + "; TRUE");
    }
    else {
        ItunesInterface::appendOutputLog("StartsWith output: " + prefix + " is not a prefix to " + root + "; FALSE");
    }
}

void ItunesInterface::dot_endsWith(vector<string> args) {
    if (args.size() < 3) {
        ItunesInterface::appendOutputLog("ERROR: .endsWith needs at least two arguments");
        return;
    }
    cout << "ALERT: spacing in double quoted arguments will not be trimmed" << endl;
    string root = args[1];
    string suffix = args[2];
    if (suffix.length() > root.length()) {
        ItunesInterface::appendOutputLog("ERROR: .endsWith suffix longer than root");
        return;
    }
    if (root.substr(root.length() - suffix.length(),suffix.length()) == suffix) {
        ItunesInterface::appendOutputLog("EndsWIth output: " + suffix + " is a suffix to " + root + "; TRUE");
    }
    else {
        ItunesInterface::appendOutputLog("EndsWIth output: " + suffix + " is not a suffix to " + root + "; FALSE");
    }
}

void ItunesInterface::dot_toTitleCase(vector<string> args) {
    if (args.size() < 2) {
        ItunesInterface::appendOutputLog("ERROR: .toTitleCase needs at least one argument");
        return;
    }
    args.erase(args.begin());
    for (int i = 0; i < args.size(); i++) {
        for (int j = 0; j < args[i].length(); j++) {
            args[i][j] = tolower(args[i][j]);
        }
    }
    if (args[0] == "the") {
        args[args.size() - 1] += ",";
        args.erase(args.begin());
        args.push_back("the");
    }
    for (int i = 0; i < args.size(); i++) {
        args[i][0] = toupper(args[i][0]);
    }

    string titlecase = "";
    cout << "ALERT: Double quoted strings are not delimited, use unquoted arguments to format" << endl;
    for (int i = 0; i < args.size(); i++) {
        titlecase += args[i];
        if (i < args.size() - 1) {
            titlecase += " ";
        }
    }
    ItunesInterface::appendOutputLog("Titlecase output: " + titlecase);
}