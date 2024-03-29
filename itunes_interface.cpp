#include "itunes_interface.hpp"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include "user.hpp"
#include "playlist.hpp"
#include "song.hpp"
#include "recording.hpp"
#include "track.hpp"

using namespace std;

vector<string> ItunesInterface::parseCommands(string arg) {
    vector<string> v;
    int indexOfInterest = 0;

    // truncates inline comment and includes in command log if not in read script mode
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

    // traverses string input
    // treats quoted arguments as block, unquoting them
    // treats unquoted chains of non-space text as block
    // seperates blocks of text by delimiting and ignoring spacing
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
    else if (cmd == ".log") {
        ItunesInterface::dot_log(v);
    }
    else if (readingMode == ENUM_READ::READING && cmd[0] == '.') {
        ItunesInterface::appendOutputLog("WARNING: Developer command not scriptable: " + cmd);
    }
    else if (cmd == ".help") {
        ItunesInterface::dot_help();
    }
    else if (cmd == ".read") {
        ItunesInterface::dot_read(v);
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
    else if (cmd == ".toTitleCase" && v.size() > 1){
        ItunesInterface::dot_toTitleCase(v[1]);
    }
    else {
        ItunesInterface::appendOutputLog("ERROR: Invalid command: " + cmd);
    }
}

void ItunesInterface::add(vector<string> args) {
    if (args.size() < 2) {
        ItunesInterface::appendOutputLog("ERROR: more arguments required for add cmd");
        return;
    }
    if ( args[1] == "-r" ) {
        if (args.size() < 7) {
            ItunesInterface::appendOutputLog("ERROR: missing args for add -r aid title artist producer year");
        }
        else {
            istringstream ss(args[2]);
            int temp_id;
            if (ss >> temp_id) {
                if (recordings.find(temp_id) == recordings.end()) {
                    istringstream ss2(args[6]);
                    int year;
                    if (ss2 >> year) {
                        recordings[temp_id] = make_shared< Recording > (
                            dot_toTitleCase(args[3]),
                            dot_toTitleCase(args[4]),
                            dot_toTitleCase(args[5]),
                            year
                        );
                    }
                    else {
                        ItunesInterface::appendOutputLog("ERROR: cannot parse year as int");
                    }
                }
                else {
                    ItunesInterface::appendOutputLog("ERROR: recording with given id already in memory");
                }
            }
            else {
                ItunesInterface::appendOutputLog("ERROR: cannot parse recording id as int");
            }
        }
    }
    else if (args[1] == "-s" ) {
        if (args.size() < 5) {
            ItunesInterface::appendOutputLog("ERROR: more args required for add -s sid title composer");
        }
        else {
            istringstream ss(args[2]);
            int temp_id;
            if (ss >> temp_id) {
                if (songs.find(temp_id) == songs.end()) {
                    songs[temp_id] = make_shared< Song > (
                        dot_toTitleCase(args[3]),
                        dot_toTitleCase(args[4])
                    );
                }
                else {
                    ItunesInterface::appendOutputLog("ERROR: song with given id already in memory");
                }
            }
            else {
                ItunesInterface::appendOutputLog("ERROR: cannot parse song id as int");
            }
        }
    }
    else if (args[1] == "-t" ) {
        if (args.size() < 6) {
            ItunesInterface::appendOutputLog("ERROR: more args required for add -t tid aid sid track_number");
        }
        else {
            vector<int> v {0,0,0,0};
            int count = 0;
            for (auto it = args.begin() + 2; it != args.end(); ++it ) {
                istringstream ss(*it);
                if (ss >> v[count]) {

                }
                else {
                    ItunesInterface::appendOutputLog("ERROR: arguments invalid");
                    return;
                }
                ++count;
            }
            if (tracks.find(v[0]) == tracks.end()) {
                if (recordings.find(v[1]) == recordings.end()) {
                    ItunesInterface::appendOutputLog("ERROR: cannot find album id");
                    cout << v[1] << endl;
                }
                else {
                    if (songs.find(v[2]) == songs.end()) {
                        ItunesInterface::appendOutputLog("ERROR: cannot find song id");
                    }
                    else {
                        if (v[3] > recordings[v[1]]->size() + 1 || v[3] < 1) {
                            ItunesInterface::appendOutputLog("ERROR: out of bounds insert on album");
                        }
                        else {
                            tracks[v[0]] = make_shared< Track >( songs[v[2]] );
                            recordings[v[1]]->insertTrack(v[3]-1, tracks[v[0]], recordings[v[1]]);
                            tracks[v[0]]->getSong().lock()->addTrack(tracks[v[0]]);
                        }
                    }
                }
            }
            else {
                ItunesInterface::appendOutputLog("ERROR: track id already exists");
            }
        }
    }
    else if (args[1] == "-u" ) {
        if (args.size() < 4) {
            ItunesInterface::appendOutputLog("ERROR: requires user_id name");
        }
        else {
            if (users.find(args[2]) == users.end()) {
                users[args[2]] = make_shared< User > (args[3]);
            }
            else {
                ItunesInterface::appendOutputLog("ERROR: user_id already taken");
            }
        }
    }
    else if (args[1] == "-p" ) {
        if (args.size() < 4) {
            ItunesInterface::appendOutputLog("ERROR: requires playlist_name");
        }
        else {
            if (users.find(args[2]) == users.end()) {
                ItunesInterface::appendOutputLog("ERROR: user not found: " + args[2]);
            }
            else {
                if (users[args[2]]->getPlaylist(args[3]).lock()) {
                    ItunesInterface::appendOutputLog("ERROR: Playlist already exists: " + args[3]);
                }
                else {
                    users[args[2]]->makePlaylist(args[3], users[args[2]]);
                }
            }
        }
    }
    else if (args[1] == "-l" ) {
        if (args.size() < 5) {
            ItunesInterface::appendOutputLog("ERROR: missing args for add -l uid playlist_name tid");
        }
        else {
            istringstream ss(args[4]);
            int temp_id;
            if (ss >> temp_id ) {
                if (users.find(args[2]) == users.end()){
                    ItunesInterface::appendOutputLog("ERROR: could not find user: " + args[2]);
                }
                else {
                    auto temp = users[args[2]]->getPlaylist(args[3]).lock();
                    if (temp) {
                        if (tracks.find(temp_id) == tracks.end()) {
                            ItunesInterface::appendOutputLog("ERROR: could not find track");
                        }
                        else {
                            temp->appendTrack(tracks[temp_id], temp);
                        }
                    }
                    else {
                        ItunesInterface::appendOutputLog("ERROR: could not find playlist: " + args[3]);
                    }
                }
            }
            else {
                ItunesInterface::appendOutputLog("ERROR: could not parse track id as int");
            }
        }
    }
    else {
        ItunesInterface::appendOutputLog("ERROR: flag not supported: " + args[1]);
    }
}

void ItunesInterface::show(vector<string> args) {
    if (args.size() < 2) {
        ItunesInterface::appendOutputLog("ERROR: more arguments required for show cmd");
        return;
    }
    if (args[1] == "users") {
        for (auto it = users.begin(); it != users.end(); ++it ) {
            cout << "==USER==" << endl;
            cout << "ID: " << it->first << endl;
            cout << *(it->second) << endl;
        }
    }
    else if (args[1] == "playlists") {
        if (args.size() < 4) {
                ItunesInterface::appendOutputLog("ERROR: missing arguments for show playlists -u userid");
                return;
        }
        if (args[2] == "-u") {
                if (users.find(args[3]) == users.end()) {
                    ItunesInterface::appendOutputLog("ERROR: user not found: " + args[3]);
                }
                else {
                    users[args[3]]->printPlaylists();
                }
        }
        else {
            ItunesInterface::appendOutputLog("ERROR: missing -u argument");
        }
    }
    else if (args[1] == "songs") {
        if (args.size() > 5) {
            string userId;
            string playlistName;
            if (args[2] == "-u" && args[4] == "-p") {
                userId = args[3];
                playlistName = args[5];
            }
            else if (args[4] == "-u" && args[2] == "-p") {
                userId = args[5];
                playlistName = args[3];
            }
            else {
                ItunesInterface::appendOutputLog("ERROR: invalid flags");
                return;
            }
            if (users.find(userId) == users.end()) {
                ItunesInterface::appendOutputLog("ERROR: cannot find user id");
                return;
            }
            auto pl = users[userId]->getPlaylist(playlistName).lock();
            if (pl) {
                cout << "==Playlist=="<<endl;
                cout << (*pl) << endl;
            }
            else {
                ItunesInterface::appendOutputLog("ERROR: cannot find playlist");
            }
        }
        else if (args.size() > 2) {
            ItunesInterface::appendOutputLog("ERROR: need more arguments show songs -u user -p playlist");
        }
        else {
            for (auto it = songs.begin(); it != songs.end(); ++it ) {
                cout << "==SONG==" << endl;
                cout << "ID: " << it->first << endl;            
                cout << *(it->second) << endl;            
            }
        }
    }
    else if (args[1] == "recordings") {
        for (auto it = recordings.begin(); it != recordings.end(); ++it ) {
            cout << "==ALBUM==" << endl;
            cout << "ID: " << it->first << endl;            
            cout << *(it->second) << endl;            
        }
    }
    else if (args[1] == "tracks") {
        for (auto it = tracks.begin(); it != tracks.end(); ++it ) {
            cout << "==TRACK==" << endl;
            cout << "ID: " << it->first << endl;            
            cout << *(it->second) << endl;            
        }
    }
    else {
        ItunesInterface::appendOutputLog("ERROR: invalid flag");
    }
}

void ItunesInterface::del(vector<string> args) {
    if (args.size() < 3) {
        ItunesInterface::appendOutputLog("ERROR: more arguments required for delete cmd");
        return;
    }
    if (args.size() == 3 && args[1] == "-t") {
        istringstream ss(args[2]);
        int id;
        if (ss >> id) {
            if ( tracks.find(id) == tracks.end() ) {
                ItunesInterface::appendOutputLog("ERROR: track id not found");
            }
            else {
                tracks[id]->purgeTrack( tracks[id] );
                tracks.erase(id);
            }
        }
        else {
            ItunesInterface::appendOutputLog("ERROR: cannot parse track id as int");
        }
    }
    else if (find(args.begin(), args.end(), "-Global") != args.end()) {
        if (args.size() < 8) {
            ItunesInterface::appendOutputLog("ERROR: missing args for delete -Global -t tid -u uid -p playlist_name");
            return;
        }
        auto it = find(args.begin(), args.end(), "-t");
        if (it == args.end()) {
            it = find(args.begin(), args.end(), "-s");
        }
        if (it == args.end() || distance(it,args.begin()) == args.size() - 1) {
            ItunesInterface::appendOutputLog("ERROR: no tid given");
            return;
        }
        istringstream ss(*(++it));
        int tempid;
        if (ss >> tempid) {
            if (tracks.find(tempid) == tracks.end()) {
                ItunesInterface::appendOutputLog("ERROR: could not parse tid as int");
                return;
            }
            tracks[tempid]->purgeTrack( tracks[tempid] );
            tracks.erase(tempid);
        }
        else {
            ItunesInterface::appendOutputLog("ERROR: could not parse tid as int");
        }
    }
    else if (args.size()==7) {
        auto it = find(args.begin(), args.end(), "-t");
        if (it == args.end()) {
            it = find(args.begin(), args.end(), "-s");
        }
        if (distance(args.begin(), it) > args.size() - 2) {
            ItunesInterface::appendOutputLog("ERROR: missing -s or -t id");
            return;
        }
        istringstream ss(*(++it));
        int trackid;
        if (!(ss >> trackid)) {
            ItunesInterface::appendOutputLog("ERROR: cannot parse tid as int");
            return;
        }
        if (tracks.find(trackid) == tracks.end()) {
            ItunesInterface::appendOutputLog("ERROR: cannot find track");
            return;
        }
        it = find(args.begin(), args.end(), "-p");
        if (distance(args.begin(), it) > args.size() - 2) {
            ItunesInterface::appendOutputLog("ERROR: missing -p playlistname");
            return;
        }
        string plname = *(++it);
        it = find(args.begin(), args.end(), "-u");
        if (distance(args.begin(), it) > args.size() - 2) {
            ItunesInterface::appendOutputLog("ERROR: missing -u userId");
            return;
        }
        string uid = *(++it);
        if (users.find(uid) == users.end()) {
            ItunesInterface::appendOutputLog("ERROR: cannot find user");
            return;
        }
        auto pl = users[uid]->getPlaylist(plname).lock();
        if (!pl) {
            ItunesInterface::appendOutputLog("ERROR: cannot find playlist");
            return;
        }
        pl->delinkTrack( tracks[trackid] );
        tracks[trackid]->delinkPlaylist(pl);
    }
    else if (args[1] == "-s") {
        istringstream ss(args[2]);
        int songid;
        if (!(ss >> songid)) {
            ItunesInterface::appendOutputLog("ERROR: cannot parse song id as int");
            return;
        }
        if (songs.find(songid) == songs.end()) {
            ItunesInterface::appendOutputLog("ERROR: cannot find song");
            return;
        }
        auto songTracks = songs[songid]->getTracks();
        for (auto it = songTracks.begin(); it != songTracks.end(); ++it ) {
            auto w = (*it).lock();
            w->purgeTrack(w);
            for (auto t = tracks.begin(); t != tracks.end(); ++t ){
                if (w==(t->second)) {
                    tracks.erase(t);
                    break;
                }
            }
            songs.erase(songid);
        }
    }
    else if (args[1] == "-r") {
        istringstream ss(args[2]);
        int recordingid;
        if (!(ss >> recordingid)) {
            ItunesInterface::appendOutputLog("ERROR: cannot parse recording id as int");
            return;
        }
        if (recordings.find(recordingid) == recordings.end()) {
            ItunesInterface::appendOutputLog("ERROR: cannot find recording");
            return;
        }
        recordings[recordingid]->purge(recordings[recordingid]);
        recordings.erase(recordingid);
    }
    else if (args.size() == 5) {
        auto pl = find(args.begin(), args.end(), "-p");
        if (distance(args.begin(), pl) > args.size() - 2) {
            ItunesInterface::appendOutputLog("ERROR: missing -p playlist");
            return;
        }
        auto uid = find(args.begin(), args.end(), "-u");
        if (distance(args.begin(), uid) > args.size() - 2) {
            ItunesInterface::appendOutputLog("ERROR: missing -u username");
            return;
        }
        pl++;
        uid++;
        if (users.find(*uid) == users.end()) {
            ItunesInterface::appendOutputLog("ERROR: cannot find user");
            return;
        }
        auto foundpl = users[*uid]->getPlaylist(*pl).lock();
        if (!(foundpl)) {
            ItunesInterface::appendOutputLog("ERROR: cannot find playlist");
            return;
        }
        auto foundUser = users[*uid];
        foundUser->deletePlaylist(foundpl);
    }
    else if (args[1] == "-u") {
        if (users.find(args[2]) == users.end()) {
            ItunesInterface::appendOutputLog("ERROR: cannot find user");
            return;
        }
        auto pls = users[args[2]]->getPlaylists();
        for (auto it = pls.begin(); it != pls.end(); ++it ) {
            users[args[2]]->deletePlaylist(*it);
        }
        users.erase(args[2]);
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
    cout<<"     Comments may appear in command script file but they will not be logged."<<endl;
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
        loggingContainerVec.push_back("INPUT: " + arg);
    }
}

void ItunesInterface::appendOutputLog(string arg) {
    cout << arg << endl;
    if (loggingMode == ENUM_LOG::OUTPUT_ONLY || loggingMode == ENUM_LOG::FULL) {
            loggingContainerVec.push_back("OUTPUT: " + arg);
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
    cout << "WARNING: Spaces in double quoted strings are not trimmed, use unquoted arguments to trim" << endl;
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
    cout << "WARNING: spacing in double quoted arguments will not be trimmed" << endl;
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
    cout << "WARNING: spacing in double quoted arguments will not be trimmed" << endl;
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

string ItunesInterface::dot_toTitleCase(string s) {
    string build = "";
    bool spaced = s[0]==' ';
    for ( int i = 0; i < s.length(); i++ ) {
        if (spaced) {
            if (s[i]==' ') {
                spaced = true;
            }
            else {
                spaced = false;
                build += toupper(s[i]);
            }
        }
        else {
            if (s[i] == ' ') {
                spaced = true;
            }
            else {
                spaced = false;
            }
            if (i == 0) {
                build += toupper(s[i]);
            }
            else {
                build += tolower(s[i]);
            }
        }
    }
    if (build[build.length() - 1] == ' ') {
        build = build.substr(0,build.length() - 1);
    }
    if (build.length() > 4 && build.substr(0,4) == "The ") {
        build = build.substr(4, build.length()-4);
        build += ", The";
    }
	return build;
}
