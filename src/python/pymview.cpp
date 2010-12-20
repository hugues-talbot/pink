/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) Hugues Talbot, 2009-2011 Université Paris-Est, Laboratoire
  d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy
  le Grand CEDEX

  hugues.talbot@gmail.com

*/

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "pink_python.h"

#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in pymview.cpp: " << msg; call_error(fullmessage.str());}

using namespace boost::python;
using namespace pink;

namespace pink {
    namespace python {

        // globals
        std::string imviewcmd("imview -server -fork -portfile ");
        int             imview_window = 0;
        
        template <class image_type> void imview (const image_type & image)
        {
            const char *directory = getenv("TMPDIR"); // should work on most unices
            if (directory == NULL) {
                directory = "/tmp"; // but just in case
            }

            // calling command
            std::ostringstream command ;
            command << imviewcmd ;
            std::ostringstream portfilename;
            // build a deterministic unique file name
            portfilename << directory << "/" << imview_window << "_" << getpid();

            // append port filename to command
            command << portfilename.str();

            // show command
            std::cerr << command.str() << std::endl;

            // remove FIFO
            unlink(portfilename.str().c_str()); // this may fail but doesn't matter.
            
            // create portfile
            int res = mkfifo(portfilename.str().c_str(), S_IRUSR | S_IWUSR);
            if (res != 0) {
                std::cerr << "Failed to create FIFO " << portfilename.str() << endl ;
                return;
            }

            /* open the pipe for reading, notice the NO DELAY  */
            int filenum = -1;
            filenum = open(portfilename.str().c_str(), O_RDONLY | O_NDELAY, 0);
            if (filenum < 0) {
                std::cerr << "Failed to open FIFO" <<  portfilename.str() << endl ;
                return;
            }

            // call imview !
            system(command.str().c_str());

            char buffer[5] = {0};

            if (read(filenum, buffer, 4) <= 0) {
                std::cerr << "Failed to read the portnumber" << endl;
                return;
            }
            close(filenum);
            unlink(portfilename.str().c_str());
            
            std::cerr << "This will show an image eventually, using port " << buffer << std::endl;
        }

    } /* namespace python */
} /* namespace pink */


UI_EXPORT_FUNCTION(
    imview,
    pink::python::imview,
    (arg("src")
        ),
    "Displays an image"
    );
