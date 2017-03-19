/*
MaikazeMizune Logger System[C++]
@Author: https://github.com/amarillys
@Email: 719862760@qq.com
@Email: kami@maikaze.win
2017/03/16 big change
2017/03/17 set some api
TO-DO: Add Linux Support
*/
#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

/*for getting the path of temp folder*/
#ifdef WIN32
#include <Windows.h>
TCHAR PATH[255];
#endif

#define FILEMACRO __FILE__
#define LINEMACRO __LINE__

#define LOG_REC 0x0000
#define LOG_SUC 0x0001
#define LOG_ERR 0x00FF
/*the parameter or valurables is invalid*/
#define LOG_NOT 0x00FE

#define ToString(x) std::to_string(x)
#define vname_(x) #x
#define vname(x) vname_(x)
using stdstr = std::string;

/*LOG_LEVEL Definition
    SUCCESS: бнбн
    WARNING: can be ingorned but nor recommanded, it don't affect the normal running.
    LOGERR: it should be solved.
    BROKEN: very serious wrong, you must solve it.
*/
enum LOG_LEVEL
{
    RECORD = 0,
    SUCCESS = 1,
    WARNING = 2,
    LOGERR = 3,
    BROKEN = 4,
};

/*Convertor function(to transform LEVEL from enum to String*/

#define LevelToStringCase(level) case level: return vname(level)

inline stdstr LEVEL2STR(LOG_LEVEL p_level)
{
    switch (p_level)
    {
		LevelToStringCase(RECORD);
		LevelToStringCase(SUCCESS);
		LevelToStringCase(WARNING);
		LevelToStringCase(BROKEN);
    case LOGERR:          return "ERROR";
    default:        return "SUCCESS";
    }
}

class Log
{
public:
    /*the temp string that save the log string.*/
    stdstr log_inf;

    /*Init Function
        @advice I suggest that call this function on creating logger or directly creating logger with parameters.
        ->like this Log* LOG = new Log(true, "d:/maikaze.log");
        @param bool p_autosave is that bool variable that decide the logger will save the log to file or not.
        @param stdstr p_path decide the path of the log file.
        @attention:
            a. if tha path is unavailable, it will try to save in temp path.
            b. if failed to save in temp path, the logg will never save.
    */
    void Init(bool p_autosave, stdstr p_path);

    /*Setting Function
        @guide the little function that change the frequence, with safety check.
    */
    void SetSaveFreq(int p_freq);
    void SetDisplay(bool p_display);

    /*Default Construction Function(!strongle not recommanded)
        @advise this construction function doesn't have any setting with saving module.
            we decide that the default value is :
    */
    Log();

    /*Construction Function
        @guide the default value of frequence we set 100.
            the main impl and guides is in Init();
    */
    Log(bool p_autosave, stdstr p_path);

    /*Main Logging Fuction
        @guide only support string type
        @if the LEVEL is LOGERR, the logger will pause the program.
        @if the LEVEL is BROKEN, the logger will stop the program and exit it.
        @parameter when the p_save is true, the log will be saved even if the autosave is false.
    */
    void log(LOG_LEVEL p_level, int p_code, stdstr p_detail);
    void log(LOG_LEVEL p_level, int p_code, stdstr p_detail, const char* p_file, int p_line);
    void log(LOG_LEVEL p_level, int p_code, stdstr p_detail, const char* p_file, int p_line, bool p_save);

    void Err(int p_code, stdstr p_deteail, const char* p_file, int p_line);
    void Rec(stdstr p_detail);
    void Rec(stdstr p_detail, const char* p_file, int p_line);
    void Recv(stdstr p_name, stdstr p_value);
    void Recv(stdstr p_name, stdstr p_value, const char* p_file, int p_line);
    void Warn(int p_code, stdstr p_deteail, const char* p_file, int p_line);
    void Boom(int p_code, stdstr p_deteail, const char* p_file, int p_line);
    /*@True Saving Function (with safety check)
        @guide Save the file to harddisk, by closing the log file and re-open it.
    */
    void SaveRefresh();
    
    /*Unconstruction member  (with safety check)
    */    
    ~Log();

private:
    /*saving module*/
    stdstr path_;
    std::vector<stdstr> history_;
    bool autosave_;
    bool display_;
    FILE* fp_;
    /*count the times when log save(Save() called), if count_ is equal to frequence(freq_)
        the count will be clear(become zero), and call the SaveRefresh() function.*/
    int count_;
    /*decide the frequence when logger refresh.*/
    int freq_;

    /*save the log to file*/
    void Save(stdstr p_inf);
    void DealLevel(LOG_LEVEL p_level);
};

inline Log::Log()
{
    //default, but bot reconmemded.
    freq_ = 56;
    autosave_ = false;
    display_ = true;
    count_ = 0;
    log_inf = "Logger of Maikaze Constructed without Init.";
    log(SUCCESS, LOG_SUC, log_inf);
}

inline Log::Log(bool p_autosave, stdstr p_path)
{
    freq_ = 56;
    display_ = true;
    Init(p_autosave, p_path);
    log_inf = "Logger of Maikaze Constructed with Init Okay.";
    log(SUCCESS, LOG_SUC, log_inf);
}

inline void Log::Init(bool p_autosave, stdstr p_path)
{
    autosave_ = p_autosave;
    path_ = p_path;
    count_ = 0;

    fopen_s(&fp_, path_.c_str(), "a+");
    if (fp_ == nullptr)
    {
        log_inf = "Try to save in temp path because cannot open the log_file to save: " + path_;
        log(WARNING, LOG_ERR, log_inf, FILEMACRO, LINEMACRO);

#ifdef WIN32
        //get the path of temp folder
        GetTempPath(std::size(PATH), PATH);
		std::wstring TEMP_PATH = PATH;
    	TEMP_PATH += L"maikaze.log";
        char tmp[255] = { 0 };
        size_t tmpint = 0;
        //because the win api return the wchar_t[], so we use the convertion.
        wcstombs_s(&tmpint, tmp, TEMP_PATH.c_str(), wcslen(TEMP_PATH.c_str()));
        path_ = tmp;
#endif
        fopen_s(&fp_, path_.c_str(), "a+");
		
        if (fp_ == nullptr)
        {
            log_inf = "All the log will not be save on harddisk because failed to save in temp path: " + path_;
            log(LOGERR, LOG_ERR, log_inf, FILEMACRO, LINEMACRO);
            autosave_ = false;
            return;
        }

	    log_inf = "Succeed to save in temp path : " + path_;
	    autosave_ = true;
	    log(SUCCESS, LOG_SUC, log_inf, FILEMACRO, LINEMACRO);
    }
    else
    {
        log_inf = "Succeed to open the log file: " + path_;
        log(SUCCESS, LOG_SUC, log_inf, FILEMACRO, LINEMACRO);
    }
}

inline Log::~Log()
{
    log_inf = "start unconstruction of logger system";
    log(RECORD, LOG_REC, log_inf);
    history_.clear();
    //safety check
    if (fp_ != nullptr)
        fclose(fp_);
}

inline void Log::log(LOG_LEVEL p_level, int p_code, stdstr p_detail)
{
    log(p_level, p_code, p_detail, "Miss File. ", -1, autosave_);
}

inline void Log::log(LOG_LEVEL p_level, int p_code, stdstr p_detail, const char * p_file, int p_line)
{
    log(p_level, p_code, p_detail, p_file, p_line, autosave_);
}

inline void Log::log(LOG_LEVEL p_level, int p_code, stdstr p_detail, const char * p_file, int p_line, bool p_save)
{
	std::stringstream ss;
	ss << p_file << "    line:  " << p_line << "\n  ->Level: " << LEVEL2STR(p_level) << "   code: " << p_code << "   detail: " << p_detail << std::endl;
    log_inf = ss.str();
    history_.push_back(log_inf);

    if (display_)
        std::cout << log_inf;
    if (p_save)
        Save(log_inf);
    DealLevel(p_level);
}

inline void Log::DealLevel(LOG_LEVEL p_level)
{
    switch (p_level)
    {
    case LOGERR: 
        std::cout << "Error occurred, please input any thing to continue." << std::endl;
        std::cin >> log_inf; break;
    case BROKEN: 
        SaveRefresh(); exit(EXIT_FAILURE);
    default: break;
    }
}

inline void Log::Save(stdstr p_inf)
{
    if (fp_ == nullptr)
    {
        log_inf = "Saving module is not opened or failed to open the save log. Please do not Save.";
        autosave_ = false;
        log(WARNING, LOG_ERR, log_inf, FILEMACRO, LINEMACRO);
        return;
    }

    fputs(p_inf.c_str(), fp_);
    if (count_++ > freq_)
        SaveRefresh();
}

inline void Log::SaveRefresh()
{
    int ret = EOF;
    if (fp_ == nullptr)
    {
        log_inf = "Saving module is not opened or failed to open the save log. Please do not Save.";
        autosave_ = false;
        log(WARNING, LOG_ERR, log_inf, FILEMACRO, LINEMACRO);
    }
    else
    {
        ret = fclose(fp_);
    }
    /*save to harddisk, this may occur a problem, and this situaltion is very rare.
    using some methods to avoid them
    loop while the file can be saved*/
    while (ret != 0)
    {
        FILE * newfp;
        stdstr newpath;
        std::cout << "please enter the new path" << std::endl;
        std::cin >> newpath;
        fopen_s(&newfp, newpath.c_str(), "a+");
        if (newfp != nullptr)
        {
            //refresh the path of log file
            path_ = newpath;
            //save the log to file
            for each (stdstr s in history_)
            {
                fputs(s.c_str(), newfp);
            }
        }
        //get the save state
        ret = fclose(newfp);
        autosave_ = true;
    }
    fopen_s(&fp_, path_.c_str(), "a+");
    count_ = 0;

    log_inf = "SaveRefresh finished.";
    log(RECORD, LOG_REC, log_inf);
}

inline void Log::SetSaveFreq(int p_freq)
{
    if (p_freq > 0 && p_freq < USHRT_MAX)
        freq_ = p_freq;
    else
        log(LOGERR, LOG_NOT, "Invalid save frequence number.");
}

inline void Log::SetDisplay(bool p_display)
{
    display_ = p_display;
}

inline void Log::Rec(stdstr p_detail)
{
    log(RECORD, LOG_REC, p_detail);
}

inline void Log::Rec(stdstr p_detail, const char* p_file, int p_line)
{
    log(RECORD, LOG_REC, p_detail, p_file, p_line);
}

inline void Log::Recv(stdstr p_name, stdstr p_value)
{
    log(RECORD, LOG_REC, p_name + " = " + p_value);
}

inline void Log::Recv(stdstr p_name, stdstr p_value, const char* p_file, int p_line)
{
    log(RECORD, LOG_REC, p_name + " = " + p_value, p_file, p_line);
}

inline void Log::Warn(int p_code, stdstr p_deteail, const char* p_file, int p_line)
{
    log(WARNING, p_code, p_deteail, p_file, p_line);
}

inline void Log::Err(int p_code, stdstr p_deteail, const char* p_file, int p_line)
{
    log(LOGERR, p_code, p_deteail, p_file, p_line);
}

inline void Log::Boom(int p_code, stdstr p_deteail, const char* p_file, int p_line)
{
    log(BROKEN, p_code, p_deteail, p_file, p_line);
}

#define LogWithContext(level, code, detail) log(level, code, detail, FILEMACRO, LINEMACRO)
