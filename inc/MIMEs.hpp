#pragma once
#include "typedefs.hpp"
#include <map>

class MIME_types
{
    std::multimap<string, string> types;
    std::multimap<string, string> types_;

public:
    MIME_types()
    {
        init_();
        reverse_();
    }

    bool is_MIME_type(string s_type)
    {
        if (types.find(s_type) != types.end())
            return (true);
        return false;
    }

    string get_MIME_extention(string s_type)
    {
        std::multimap<string, string>::iterator it;

        it = types_.find(s_type);
        if (it != types_.end())
            return ("." + it->second);
        return "";
    }

    string get_extention(string file_name)
    {
        size_t pos;

        pos = file_name.rfind(".");
        if (pos == 0 || pos == file_name.npos)
        {
            return "no_type";
        }
        return file_name.substr(pos + 1);
    }

    string get_type(string ext)
    {
        std::multimap<string, string>::iterator it;

        it = types.find(ext);
        if (it != types.end())
        {
            return it->second;
        }
        return "";
    }

private:
    void init_()
    {
        types.insert(std::make_pair("html", "text/html"));
        types.insert(std::make_pair("htm", "text/html"));
        types.insert(std::make_pair("shtml", "text/html"));
        types.insert(std::make_pair("css", "text/css"));
        types.insert(std::make_pair("xml", "text/xml"));
        types.insert(std::make_pair("gif", "image/gif"));
        types.insert(std::make_pair("jpeg", "image/jpeg"));
        types.insert(std::make_pair("jpg", "image/jpeg"));
        types.insert(std::make_pair("js", "application/javascript"));
        types.insert(std::make_pair("atom", "application/atom+xml"));
        types.insert(std::make_pair("rss", "application/rss+xml"));
        types.insert(std::make_pair("mml", "text/mathml"));
        types.insert(std::make_pair("txt", "text/plain"));
        types.insert(std::make_pair("jad", "text/vnd.sun.j2me.app-descriptor"));
        types.insert(std::make_pair("wml", "text/vnd.wap.wml"));
        types.insert(std::make_pair("htc", "text/x-component"));
        types.insert(std::make_pair("png", "image/png"));
        types.insert(std::make_pair("tif", "image/tiff"));
        types.insert(std::make_pair("tiff", "image/tiff"));
        types.insert(std::make_pair("wbmp", "image/vnd.wap.wbmp"));
        types.insert(std::make_pair("ico", "image/x-icon"));
        types.insert(std::make_pair("jng", "image/x-jng"));
        types.insert(std::make_pair("bmp", "image/x-ms-bmp"));
        types.insert(std::make_pair("svg", "image/svg+xml"));
        types.insert(std::make_pair("svgz", "image/svg+xml"));
        types.insert(std::make_pair("webp", "image/webp"));
        types.insert(std::make_pair("woff", "application/font-woff"));
        types.insert(std::make_pair("jar", "application/java-archive"));
        types.insert(std::make_pair("war", "application/java-archive"));
        types.insert(std::make_pair("ear", "application/java-archive"));
        types.insert(std::make_pair("json", "application/json"));
        types.insert(std::make_pair("hqx", "application/mac-binhex40"));
        types.insert(std::make_pair("doc", "application/msword"));
        types.insert(std::make_pair("pdf", "application/pdf"));
        types.insert(std::make_pair("ps", "application/postscript"));
        types.insert(std::make_pair("eps", "application/postscript"));
        types.insert(std::make_pair("ai", "application/postscript"));
        types.insert(std::make_pair("rtf", "application/rtf"));
        types.insert(std::make_pair("m3u8", "application/vnd.apple.mpegurl"));
        types.insert(std::make_pair("xls", "application/vnd.ms-excel"));
        types.insert(std::make_pair("eot", "application/vnd.ms-fontobject"));
        types.insert(std::make_pair("ppt", "application/vnd.ms-powerpoint"));
        types.insert(std::make_pair("wmlc", "application/vnd.wap.wmlc"));
        types.insert(std::make_pair("kml", "application/vnd.google-earth.kml+xml"));
        types.insert(std::make_pair("kmz", "application/vnd.google-earth.kmz"));
        types.insert(std::make_pair("7z", "application/x-7z-compressed"));
        types.insert(std::make_pair("cco", "application/x-cocoa"));
        types.insert(std::make_pair("jardiff", "application/x-java-archive-diff"));
        types.insert(std::make_pair("jnlp", "application/x-java-jnlp-file"));
        types.insert(std::make_pair("run", "application/x-makeself"));
        types.insert(std::make_pair("pl", "application/x-perl"));
        types.insert(std::make_pair("pm", "application/x-perl"));
        types.insert(std::make_pair("prc", "application/x-pilot"));
        types.insert(std::make_pair("pdb", "application/x-pilot"));
        types.insert(std::make_pair("rar", "application/x-rar-compressed"));
        types.insert(std::make_pair("rpm", "application/x-redhat-package-manager"));
        types.insert(std::make_pair("sea", "application/x-sea"));
        types.insert(std::make_pair("mp4", "video/mp4"));
    } 
    void reverse_()
    {
        for (std::multimap<string, string>::iterator it = types.begin(); it != types.end(); it++)
        {
            types_.insert(std::make_pair(it->second, it->first));
        }
    }

};
