#ifndef FILESPATHHANDLER_HPP
#define FILESPATHHANDLER_HPP

#include <map>
#include <string>
#include <boost/filesystem.hpp>

typedef std::map<const std::string, std::string> map_filesPath;

/** \brief A class that manages files' alias and complete path.
*/
class FilesPathHandler
{
    public:
        FilesPathHandler() { }
        ~FilesPathHandler() { }

        /** \brief Adds a file (or updates if existing).
        *
        * \param alias File's alias (ex : "image.png").
        * \param path File's path (ex : "arts/image.png").
        */
        void addFile(const std::string &alias, const std::string &path)
        {
            addFile(alias, path, true);
        }
        /** \brief Adds a file (or updates if required).
        *
        * \param alias File's alias (ex : "image.png").
        * \param path File's path (ex : "arts/image.png").
        * \param replace Update if alias already exists.
        */
        void addFile(const std::string &alias, const std::string &path,
            const bool &replace)
        {
            map_filesPath::iterator iter = find(alias);
            if (iter == filespath.end())
                filespath.insert(make_pair(alias, path));
            else if (replace)
                iter->second = path;
        }

        /** \brief Searchs for a file.
        *
        * \param alias File's alias to find.
        * \return File's fullpath if found, or an empty string else.
        */
        std::string getFilepath(const std::string &alias) const
        {
            const map_filesPath::const_iterator iter = find(alias);
            if (iter != filespath.end())
                return iter->second;
            return "";
        }
        /** \brief Operator ().
        *
        * \see getFilepath()
        */
        std::string operator()(const std::string &alias) const
        {
            return getFilepath(alias);
        }

        static bool scanDirectory(const std::string &dir, FilesPathHandler &fph)
        {
            namespace fs = boost::filesystem;
            if (!fs::exists(dir))
            {
                //gLog << logH << "Error : folder '" << imgdir << "' does not exist.\n";
                return false;
            }
            const fs::path path(dir);

            fs::directory_iterator itEnd;
            for (fs::directory_iterator it(dir); it != itEnd; ++it)
            {
                if (fs::is_directory(it->status()))
                {
                    if (!scanDirectory(it->path().string(), fph))
                        return false;
                }
                else if (fs::is_regular(it->status()))
                    fph.addFile(it->filename(), it->string(), true);
            }
            return true;
        }

    private:
        inline map_filesPath::iterator find(const std::string &alias)
        {
            return filespath.find(alias);
        }
        inline map_filesPath::const_iterator find(const std::string &alias) const
        {
            return filespath.find(alias);
        }

        map_filesPath filespath;
};

extern FilesPathHandler gFph;

#endif /* FILESPATHHANDLER_HPP */
