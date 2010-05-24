/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2009-2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/



#include  <pink.h>

using namespace pink;

#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in uiSqhool.cpp: " << msg; call_error(fullmessage.str());}

#define sqlite3(command, errormessage)				      \
 if (SQLITE_OK!=command){                                             \
   cout << "sqlite says: " << sqlite3_errmsg(database) << endl;       \
   error(errormessage);                                               \
 };


int atoi( const string & src ){
  int result;
  stringstream ss;
  ss << src;
  ss >> result;
  return result;
};

int atoi( char * src ){
  int result;
  stringstream ss;
  ss << src;
  ss >> result;
  return result;
};


// int strlen(const char *str)
// {
//   const char *s;
  
//   for (s = str; *s; ++s);
  
//   return (s - str);
// }


uiSqhool::uiSqhool ( ){
// intentionally left empty
};

uiSqhool::~uiSqhool ( ){
  sqlite3(sqlite3_close(database), "couldn't close the database.")

  cout << "Database closed" << endl;
};

uiSqhool::uiSqhool ( const string & filename, const string & creator, const string & description, bool create){
  init(filename, creator, description, create);
};

void uiSqhool::init ( const string & filename, const string & creator, const string & description, bool create ){
  //TODO: error checking and handling

  ppchar error_message=NULL;

  // opening the database
  if (create){
    cout << "Creating new file" << endl;
    try{
      remove(filename.c_str());
    } 
    catch (...) {};

    //creating a new file and attaching an sqlite to it
    sqlite3(sqlite3_open_v2( filename.c_str(), &database, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL ), 
	    "create a new file");
    
    // creating the basic structure of tables and dependencies. References are as of 2009. jan ignored
    sqlite3(sqlite3_exec( database, uiCreateFile.c_str() /*a predefined command*/, &callback, NULL, error_message ),
	    "couldn't create the default tables in the file");

// "  creation_date TEXT , "
// "  last_modified TEXT , "
// "  ujif_version TEXT  DEFAULT  \"4.1.1.0 \", "
// "  description TEXT , "
// "  creator TEXT , "
// "  file_id INTEGER primary key  unique  DEFAULT 1, "
// "  current_image INTEGER  "

    ///// TIME GENERATION ----------------------------------
    stringstream currtime;

    {
      time_t rawtime;
      struct tm * timeinfo;
      
      time ( &rawtime );
      timeinfo = localtime ( &rawtime );
      char * ctime = asctime(timeinfo);
      ctime[strlen(ctime)-1]=0;
      currtime << ctime; //asctime(timeinfo);
    };
    

    stringstream ss;
    ss << "insert into fileinfo(creation_date, description, creator, file_id) values ("
       << "'" << currtime.str().c_str()  << "',"
       << "'" << description.c_str()     << "',"
       << "'" << creator.c_str()         << "'," 
       << "1"
       << ");";
    sql_execute(ss);

  } else {
    cout << "Opening for read" << endl;
    sqlite3( sqlite3_open_v2(filename.c_str(), &database, SQLITE_OPEN_READWRITE, NULL ) , 
	     "couldn't open the file"
      );
  };

  cout << "file:\"" << filename << "\" opened" << endl;

};

// // to be optimized!!!
// PTR<uiScalarField> uiSqhool::getImage( int ID ){
//   progressBar sentinel;
//   sentinel.start();

//   char ** results;
//   int row;
//   int column;
//   char * errmsg;

//   // reading the dimensions of the image from the file
//   stringstream ss;
//   ss << "select size_x,size_y,size_z,size_t,data_type from images where id=" << ID << ";";

//   sqlite3_get_table(
//     database,          // sqlite3 *db,          /* An open database */
//     ss.str().c_str(),  // const char *zSql,     /* SQL to be evaluated */
//     &results,          // char ***pazResult,    /* Results of the query */
//     &row,              // int *pnRow,           /* Number of result rows written here */
//     &column,           // int *pnColumn,        /* Number of result columns written here */
//     &errmsg            // char **pzErrmsg       /* Error msg written here */
//     );

//   int x,y,z,t;
//   stringstream sstype;
//   x=atoi(results[5+0]);
//   y=atoi(results[5+1]);
//   z=atoi(results[5+2]);
//   t=atoi(results[5+3]);
//   sstype << results[5+4];
//   sqlite3_free_table(results);

//   PTR<vint> dim = getDimensions(x,y,z,t);
//   int d = dim->size();
// //   FOR (q,int(dim.size())){
// //     cout << "dim[" << q << "]=" << dim[q] << endl;
// //   };

//   cout << "getting a '" << sstype.str() << "' image, dimension is " << d << "D (x=" << x << ", y=" << y << ", z=" << z << ", t=" << t << ")" << endl;

//   PTR<uiScalarField> presult(new uiScalarField(dim));
//   uiScalarField & result = *presult;

//   // getting the rowid's of the lines, that contain the image
  
//   stringstream ss2;
//   ss2 << "select rowid,pos_x,pos_y,pos_z,pos_t,size_x,size_y,size_z,size_t from datas where id=" << ID << ";";
//   sqlite3 (sqlite3_get_table( database, ss2.str().c_str(), &results, &row, &column, &errmsg), 
// 	   "couldn't get the list of rowid s which containt the blobs");

//   FOR(q, row){ // the ids are from 1 to row, that is q+1
//     sqlite3_int64 iRow;
//     PTR<vint> pos  = readDataDetails( results, 9*(q+1)+1, d );
//     PTR<vint> size = readDataDetails( results, 9*(q+1)+5, d );

//     FOR(w,int(size->size())) cout << "(*size)[" << w << "]=" << (*size)[w] << endl;

//     iRow = atoi(results[9*(q+1)+0]);
    
//     psqlite3_blob blob;

//     sqlite3( sqlite3_blob_open( database, "main", "datas", "data", iRow, OPEN_BLOB_RO, &blob ),
// 	     "couldn't open the blob from the datas table in the image file");

//     if (sstype.str()=="BYTE") {
//       ARRAY<unsigned char> data (new unsigned char[size->prod()]);
      
//       if (sqlite3_blob_bytes(blob) != int(sizeof(unsigned char)*size->prod()))
// 	error("the size of the data is not corresponding with the size defined by the tags");
      
      
//       sqlite3( sqlite3_blob_read(blob, (void*)data.get(), sizeof(unsigned char)*size->prod(), 0), 
// 	       "the data is not read");
      
//       uiScalarField tmp( *size, data);

//       result.apply(tmp, *pos);










//     } else if (sstype.str()=="FLOAT32") {
//       if (sizeof(float)!=4){ error("the float is not 32 bit on this system, the image reading would not work"); };

//       ARRAY<float> data(new float[size->prod()]);
      
//       if (sqlite3_blob_bytes(blob) != int(sizeof(unsigned char)*size->prod()))
// 	error("the size of the data is not corresponding with the size defined by the tags");
      
//       sqlite3( sqlite3_blob_read(blob, (void*)data.get(), sizeof(unsigned char)*size->prod(), 0),
// 	       "the data is not read");
      
//       uiScalarField tmp(*size, data);

//       result.apply(tmp, *pos);

//     } else {
//       cout << "datatype= '" << sstype.str() << "'" << endl;
//       error("unknown data_type. Note: valid types are BYTE and FLOAT32");
//     };

//     sqlite3( sqlite3_blob_close(blob) , "couldn't close the blob");

//   };

//   sqlite3_free_table(results);

//   cout << "Image read in " << sentinel.elapsedTime() << endl;
//   return presult;
// };

PTR<vint> uiSqhool::read_data_details( char ** results, int pos , int d ){
  PTR<vint> presult (new vint(d,-1));
  vint & result = *presult;

  if ((d<=1) or (d>=4)) {error("read error: dimension wrong or unsupported");}

  FOR(q,d)
    result[q]=atoi(results[q+pos]);

  return presult;
};

int uiSqhool::get_SQL_value(const stringstream & command){
  char ** results;
  int row;
  int column;
  char * errmsg;
  int res;

  sqlite3_get_table(
    database,         // sqlite3 *db,          /* An open database */
    command.str().c_str(),  // const char *zSql,     /* SQL to be evaluated */
    &results,         // char ***pazResult,    /* Results of the query */
    &row,             // int *pnRow,           /* Number of result rows written here */
    &column,          // int *pnColumn,        /* Number of result columns written here */
    &errmsg           // char **pzErrmsg       /* Error msg written here */
    );

  stringstream atoi(results[1]); 
  atoi >> res;
  sqlite3_free_table(results);

  return res;
};

void uiSqhool::sql_execute(const stringstream & command, string error_message){
  char ** results;
  int row;
  int column;
  char * errmsg;
  
  sqlite3(sqlite3_get_table(database, command.str().c_str(), &results, &row, &column, &errmsg ),
	  error_message);    
  
  sqlite3_free_table(results);  
};


PTR<vint> pink::get_dimensions( const int x, const int y, const int z, const int t ){
  PTR<vint> presult;
  if (t>1) {
    /////!!!!!!! cout<< "I've desided for 4D." << endl;
    presult.reset(new vint(4,-1));
    (*presult)[0]=x;
    (*presult)[1]=y;
    (*presult)[2]=z;
    (*presult)[3]=t;
  } else if (z>1){
    /////!!!!!!! cout<< "I've desided for 3D." << endl;
    presult.reset(new vint(3,-1));
    (*presult)[0]=x;
    (*presult)[1]=y;
    (*presult)[2]=z;
  } else if (y>1){
    /////!!!!!!! cout<< "I've desided for 2D." << endl;
    presult.reset(new vint(2,-1));
    (*presult)[0]=x;
    (*presult)[1]=y;
  } else if (x>1){
    /////!!!!!!! cout<< "I've desided for 1D or less." << endl;
    error("an image should have at least 2 dimensions");
  };
  
  if ((t>1) and (z==1)){
    error("two dimensional time series are probably not handled well");
  };
  return presult;
}


void pink::set_dimensions(const vint & dim, int & x, int & y, int & z, int & t){
  int d = dim.size();
  bool result = true;
  x=y=z=t=1;
  switch (d){
  case 1:
    x=dim[0];
    y=1;
    z=1;
    t=1;
    break;
  case 2:
    x=dim[0];
    y=dim[1];
    z=1;
    t=1;
    break;
  case 3:
    x=dim[0];
    y=dim[1];
    z=dim[2];
    t=1;
    break;
  case 4:
    x=dim[0];
    y=dim[1];
    z=dim[2];
    t=dim[3];
    break;
  default:
    result = false;
    error("error: the dimension is wrong or unsupported");
    break;
  }
};

// void uiSqhool::set_image ( uiScalarField & image, const string & type, const string & data_type, const string & relative_path, string full_path=""){
//   //In a later time an image can be broken up in order to respect the 10E6 byte blob limit.

//   progressBar sentinel;
//   sentinel.start();
//   cout << "image.size.size()=" << image.size.size() << endl;
//   int x,y,z,t;
//   //setting up dimensions
//   setDimensions(image.size, x, y, z, t); // as 4 dimensions are supported
  
//   cout << "inserting image with size: (x=" << x << ", y=" << y << ", z=" << z << ", t=" << t << ")" << endl;

//   int new_id;

//   {
//     stringstream command;
//     command << "select count(id) from images;";
  
//     new_id=1+getSQLValue(command);

//     stringstream ss;     
//     ss << 
//       "insert into images (ID, size_x, size_y, size_z, size_t, type, full_path, relative_path, data_type) values (" 
//        << new_id << "," 
//        << x << "," 
//        << y << "," 
//        << z << "," 
//        << t <<  ",'"  
//        << type << "','" 
//        << full_path << "','" 
//        << relative_path << "','" 
//        << data_type << "');";
//     sqlExecute(ss);
//   };

//   //------------- the corresponding line inserted into "images" table --------------

//   //------------- inserting the data into the "datas" table ------------------------

//   {

//     stringstream ss; 
//     ss << "insert into datas (ID, data, pos_x, pos_y, pos_z, pos_t, size_x, size_y, size_z, size_t) values ("
//        << new_id 
//        << ","
//        << "?" << // the data will be compiled here
//       ",0,0,0,0,"
//        << x << "," 
//        << y << "," 
//        << z << "," 
//        << t << ");";

//     if (data_type=="BYTE"){
//       ARRAY<unsigned char> data = image.getCharRepresentation(); //boost's shared array will delete itself
//       insertBlob((void*)data.get(), image.size.prod()*sizeof(char), ss);
//     } else if (data_type=="FLOAT32"){
//       ARRAY<float> data = image.getFloatRepresentation();
//       insertBlob((void*)data.get(), image.size.prod()*sizeof(float), ss);
//     } else {
//       error("wrong type of the image to insert. Note valid types are BYTE and FLOAT32");
//     };

//   };
  
//   cout << "image inserted in " << sentinel.elapsedTime() << " with id = " << new_id << endl;
//   touch();
// };

void uiSqhool::insert_blob(void * data, const int size, const stringstream & ss){
  //this function should be parametrized later as we cut the images up

  psqlite3_stmt statement;
  char const * tail;

  cout << " size is: " << size << endl;

  sqlite3_prepare_v2(
    database,         // sqlite3 *db,            /* Database handle */
    ss.str().c_str(), // const char *zSql,       /* SQL statement, UTF-8 encoded */
    ss.str().size(),  // int nByte,              /* Maximum lengthi of zSql in bytes. */
    &statement,       // sqlite3_stmt **ppStmt,  /* OUT: Statement handle */
    &tail             // const char **pzTail     /* OUT: Pointer to unused portion of zSql */
    );


  sqlite3_bind_blob( 
    statement, 
    1, /*we want to change the first questionmark*/
    data,
    size,
    SQLITE_TRANSIENT /*SQLite makes its own private copy of the data immediately, before the sqlite3_bind_*() routine returns*/
    );

  sqlite3_step(statement);
  sqlite3_finalize(statement);

};


void uiSqhool::set_comment ( int ID, const string & comment ){
  error("setComment: this function is not implemented yet");
};

void uiSqhool::set_log ( int ID, const string & log ){
  error("setLog: this function is not implemented yet");
};

void uiSqhool::set_command ( int ID, const string & command ){
  stringstream ss;
  ss << "insert into commands(id, command, language) values ("
     << ID << "," << command
     << ");";
  sql_execute(ss, "can't insert the command into the commands table");
};

void uiSqhool::set_substitution ( int ID_final_image, int ID_source_image, const string & name_source_in_command ){
  stringstream ss;
  ss << "insert into substitutions( ID_final_image, ID_source_image, name_source_in_command ) values ( "
     << ID_final_image << "," << ID_source_image << "," << name_source_in_command 
     << ");";
    
  sql_execute(ss, "can't insert the substitution");
};

PTR<string> uiSqhool::get_command( int ID ){
  char ** results;
  int row;
  int column;
  char * errmsg;
  
  PTR<string> presult;

  stringstream ss;
  ss << "select command from commands where id=" << ID << ";";

  sqlite3(sqlite3_get_table(database, ss.str().c_str(), &results, &row, &column, &errmsg ), 
	  "couldn't get the command correspondent with that id.");

  if (row==0) 
    presult.reset(new string(""));
  else {
    presult.reset(new string(results[1]));
  };

  sqlite3_free_table(results);
  return presult;
};

PTR<vint> uiSqhool::get_dependencies( const vint & IDs ){
  char ** results;
  int row;
  int column;
  char * errmsg;


  stringstream ss;
  ss << "select ID_source_image from substitutions where ID_final_image in (" ;
  FOR(q, int(IDs.size())-1){
    ss << IDs[q] << ",";
  };
  ss << IDs[IDs.size()-1];
  ss << ");";

  cout << "ss is: '" << ss.str() << "'" << endl;
  
  sqlite3(sqlite3_get_table(database, ss.str().c_str(), &results, &row, &column, &errmsg ), 
	  "couldn't get the list of dependencies.");

  PTR<vint> presult(new vint(row));
  FOR (q, row)
    (*presult)[q]=atoi(results[q+1]);
  

  sqlite3_free_table(results);
  return presult;
};

PTR<vector<string> > uiSqhool::get_commands ( int ID ){
  PTR<vint> dependencies(new vint(1));
  (*dependencies)[0]=ID;
  
  PTR<vint> dep = get_dependencies(*dependencies);
  
  while (dependencies->addSet(*dep)) { // Here I iterate over the set of know dependencies (starting with a set {ID}).
    dep = get_dependencies(*dependencies);
  };// In the end I should have all the images that where used to create image 1;

  PTR<vector<string> > result(new vector<string>(0));
  FOR( q, int(dependencies->size()) ) {
    PTR<string> currcommand = get_command((*dependencies)[q]);
    if ( (*currcommand) != "") result->push_back( *currcommand );
  };
  
  return result;

// EXAMPLE:
//   PTR<uiSqhool> ujif(new uiSqhool("/home/ujoimro/tmp/ujif/test.ujif", "", "", false));

//   PTR<vector<string> > commands = ujif->getCommands(1);

//   cout << "my commands are:" << endl;

//   FOR(q, int(commands->size()))
//     cout << (*commands)[q] << endl;


};

PTR<vint> uiSqhool::list_images ( ) {
  char ** results;
  int row;
  int column;
  char * errmsg;

  stringstream ss;
  ss << "select id from images;";  
  
  if (SQLITE_OK!=sqlite3_get_table(database, ss.str().c_str(), &results, &row, &column, &errmsg )){
    cout << "sqlite says: " << sqlite3_errmsg(database) << endl;
    error("couldn't get the list of the images.");    
  };
  
  PTR<vint> result(new vint(row,-1));
  
  FOR(q, row) {
    (*result)[q]=atoi(results[q+1]);
  }

  sqlite3_free_table(results);

  return result;
};

void uiSqhool::touch() {
  ///// TIME GENERATION ----------------------------------
  stringstream currtime;
  
  {
    time_t rawtime;
    struct tm * timeinfo;
    
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    char * ctime = asctime(timeinfo);
    ctime[strlen(ctime)-1]=0;
    //      printf ( "Current local time and date: %s", asctime (timeinfo) );
    currtime << ctime; //asctime(timeinfo);
  };
  
  stringstream ss;
  
  ss << "update fileinfo set last_modified="
     << "'" << currtime.str().c_str()  << "' where file_id=1;";
  
  sql_execute(ss);
};

int uiSqhool::callback (void * NotUsed, int argc, ppchar argv, ppchar azColName ){

//This is a default function of SQLite
  
  NotUsed=0;
  FOR(i, argc){
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i]: "NULL");
  }
  printf("\n");
  return 0;
};


string uiSqhool::get_image_type( int ID  ){
  if (ID==-1){
    error("cannot yet choose the image automaticly");
  } 
  else /* NOT if (ID==-1) */
  {
    error("cannot yet find the image type at all");
  }; /* NOT if (ID==-1) */
};



namespace pink {
//File format 4.1.1.0

string uiCreateFile=""
"CREATE TABLE comments (  "
" "
"  ID INTEGER references images(ID), "
"  comment TEXT  "
" "
"); "
"CREATE TABLE commands (  "
" "
"  ID INTEGER references images(ID), "
"  command TEXT , "
"  language TEXT  "
"); "
"CREATE TABLE datas (  "
" "
"  ID INTEGER references images(ID), "
"  data BLOB , "
"  pos_x INTEGER ,"
"  pos_y INTEGER ,"
"  pos_z INTEGER ,"
"  pos_t INTEGER ,"
"  size_x INTEGER ,"
"  size_y INTEGER ,"
"  size_z INTEGER ,"
"  size_t INTEGER "
"); "
"CREATE TABLE fileinfo (  "
"  creation_date TEXT , "
"  last_modified TEXT , "
"  ujif_version TEXT  DEFAULT  \"4.1.1.0 \", "
"  description TEXT , "
"  creator TEXT , "
"  file_id INTEGER primary key  unique  DEFAULT 1, "
"  current_image INTEGER  "
", "
"  CHECK ( file_id=1 ) " 
"); "
"CREATE TABLE images (  "
"  ID INTEGER primary key unique, "
"  size_x INTEGER , "
"  size_y INTEGER , "
"  size_z INTEGER , "
"  size_t INTEGER , "
"  type TEXT , "
"  full_path TEXT , "
"  relative_path TEXT , "
"  data_type TEXT  "
"); "
"CREATE TABLE logs (  "
" "
"  ID INTEGER references images(ID), "
"  log TEXT  "
" "
"); "
"CREATE TABLE substitutions (  "
" "
"  ID_final_image INTEGER references images(ID) ,"
"  ID_source_image INTEGER references images(ID) ,"
"  name_source_in_command TEXT  "
" "
"); ";


// LuM end of file
}
