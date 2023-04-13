#include <typedefs.hpp>
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

    

private:
    void init_()
    {
        types.insert({"html", "text/html"});
        types.insert({"htm", "text/html"});
        types.insert({"shtml", "text/html"});
        types.insert({"css", "text/css"});
        types.insert({"xml", "text/xml"});
        types.insert({"gif", "image/gif"});
        types.insert({"jpeg", "image/jpeg"});
        types.insert({"jpg", "image/jpeg"});
        types.insert({"js", "application/javascript"});
        types.insert({"atom", "application/atom+xml"});
        types.insert({"rss", "application/rss+xml"});
        types.insert({"mml", "text/mathml"});
        types.insert({"txt", "text/plain"});
        types.insert({"jad", "text/vnd.sun.j2me.app-descriptor"});
        types.insert({"wml", "text/vnd.wap.wml"});
        types.insert({"htc", "text/x-component"});
        types.insert({"png", "image/png"});
        types.insert({"tif", "image/tiff"});
        types.insert({"tiff", "image/tiff"});
        types.insert({"wbmp", "image/vnd.wap.wbmp"});
        types.insert({"ico", "image/x-icon"});
        types.insert({"jng", "image/x-jng"});
        types.insert({"bmp", "image/x-ms-bmp"});
        types.insert({"svg", "image/svg+xml"});
        types.insert({"svgz", "image/svg+xml"});
        types.insert({"webp", "image/webp"});
        types.insert({"woff", "application/font-woff"});
        types.insert({"jar", "application/java-archive"});
        types.insert({"war", "application/java-archive"});
        types.insert({"ear", "application/java-archive"});
        types.insert({"json", "application/json"});
        types.insert({"hqx", "application/mac-binhex40"});
        types.insert({"doc", "application/msword"});
        types.insert({"pdf", "application/pdf"});
        types.insert({"ps", "application/postscript"});
        types.insert({"eps", "application/postscript"});
        types.insert({"ai", "application/postscript"});
        types.insert({"rtf", "application/rtf"});
        types.insert({"m3u8", "application/vnd.apple.mpegurl"});
        types.insert({"xls", "application/vnd.ms-excel"});
        types.insert({"eot", "application/vnd.ms-fontobject"});
        types.insert({"ppt", "application/vnd.ms-powerpoint"});
        types.insert({"wmlc", "application/vnd.wap.wmlc"});
        types.insert({"kml", "application/vnd.google-earth.kml+xml"});
        types.insert({"kmz", "application/vnd.google-earth.kmz"});
        types.insert({"7z", "application/x-7z-compressed"});
        types.insert({"cco", "application/x-cocoa"});
        types.insert({"jardiff", "application/x-java-archive-diff"});
        types.insert({"jnlp", "application/x-java-jnlp-file"});
        types.insert({"run", "application/x-makeself"});
        types.insert({"pl", "application/x-perl"});
        types.insert({"pm", "application/x-perl"});
        types.insert({"prc", "application/x-pilot"});
        types.insert({"pdb", "application/x-pilot"});
        types.insert({"rar", "application/x-rar-compressed"});
        types.insert({"rpm", "application/x-redhat-package-manager"});
        types.insert({"sea", "application/x-sea"});
    } 
    void reverse_()
    {
        for (std::multimap<string, string>::iterator it = types.begin(); it != types.end(); it++)
        {
            types_.insert({it->second, it->first});
        }
    }

};

MIME_types MIME;