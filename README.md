# study_planner

This is a Windows desktop application that I wrote for planning my studies at Aalto University. That is, planning what courses I will complete and when. This application was originally meant for private use solely by me (which explains most of the design decisions) but it grew to be a big enough project that I considered it worthwile to document it and upload it to GitHub. The application is written in C++ and utilizes QT Widgets for the UI, JSON files for file management using nlohmanns's JSON C++ library and CMake for building the project.

## Running the appliction

The application currently only supports Windows and has been tested on Windows 11 with an Intel processer only.

### Installing the application directly

There is a Windows .exe installer available in the releases section of the GitHub repository.

### Building from source

1. You need to have mingw_64-version of QT, nlohmann's C++ JSON library and CMake installed.
	1. For QT go to https://www.qt.io/development/download-qt-installer-oss and in the installer make sure to select the mingw_64-version for installation.
	2. For nlohmann's JSON library go to https://github.com/nlohmann/json and simply clone the repository to your machine.
	3. For the CMake installer refer to https://cmake.org/download/
2. In `CMakePresets.json` in the `"cacheVariables"`-section match `"Qt6_DIR"` and `"nlohmann_json_DIR"` to your installation paths of QT and nlohmann's JSON library.
3. To build, run:
	1. `cmake --preset x64-debug`
	2. `cmake --build out/build/x64-debug`
	3. `cmake --install out/build/x64-debug` (currently the build version isn't working but the installed version is)
3. You now have a working .exe file in `out/install/x64-debug/bin/study_planner.exe`.

## The UI

### Timeline

Upon starting the application, this is the largest UI component that the user sees. Initially, it only shows years, periods and 0 credits per period at the top of the timeline, but courses that the user adds to their study plan will show up on it once they are selected and planned. Planned courses (referred to as course plans from now on, see [course plan](#course-plan)) show up as blocks on the timeline that are ordered from top to bottom in the order that they were added but also fitted togehter as tightly as possible. Each block shows the code and name of the course associated with the course plan as well as how many ECTS credits are earned by completing the course. The outline of the block is black for courses where participation in teaching is planned and blue for courses where completion by only participating in an exam is planned (see [participation plan](#participation-plan) and [exam plan](#exam-plan)). Each block also has a trash can icon for deleting the course plan from the current study plan. The study plan is essentially just a list of course plans that represents a complete plan of your future studies. The years on the timeline start from 2025 and the timeline lasts four years in total as those are the years of interest to me.

### Course selector

On the right side of the UI a list of courses is shown. Each course is shown as an individual card component that is almost identical to the blocks shown on the timeline (except for the deletion icon). The list of courses can filtered by the period that the course starts in, the length of the course, whether the course is possible to complete by only participating in an exam or a case-insensitive keyword that will match the code, the name or the tags attributed to the course. The user can click a course to select it and click the "add course" button at the bottom to open the course plan dialog. After the course is planned it is removed from the course selector so that no course is planned twice. The list of available courses is stored in a JSON file (see [available courses](#available-courses)).

### Course plan dialog

Upon selecting a course and clicking the "add course" button in the course selector, the user is presented a dialog that features several options for planning the completion of the course. The user can select the year of studies that the course starts in (some courses are organised only even or odd years, see [even or odd years](#even-or-odd-years)), whether to complete the course by only participating in an exam and what period to start the course in. Only the periods where the course is organised are shown. If the user chooses to complete the course by only participating in an exam, the user can freely choose the start period as well as the length of the course plan. Year of the exam and the period of the exam (which are also limited to periods where exams are organised) have to be chosen in this case as well. Finally, clicking the "add" button, the course is added to the current study plan and shown in the timeline.

### File menu

Clicking "File" at the top left corner of the application opens the file menu, which has options to save the current study plan or load a previously saved study plan. Study plans are saved as JSON files but currently the user has to remember to add the `.json` file extension manually to the file name when saving the file.

### Bottom menu

At the bottom of the application total study points of the current study plan are shown and there is a checkbox to toggle between compact and normal mode. In compact mode, the courses on the timeline take less space vertically in order to fit more courses without having to scroll.

## The program structure

### Main function

First the main function loads the list of available courses (see [available courses](#available-courses) in the [file management](#file-management) section). It then initiates the QT widget components of the main UI and an empty study plan.

### Course

A course is modelled by a struct. I chose a struct for this purpose since a course is always just an immutable collection of a bunch of different data types.

#### Code, name and tags

Each course has a code and name implemented as strings. These are simply the code and name of an Aalto University course which every course has. There is also a vector of strings representing a list of tags for the course. The tags are freeform and their only function is that tags are matched by the keyword search. I used tags for identifying which major each course is a part of.

#### Credits

The amount of ECTS credits earned by completing the course is represented by an unsigned integer. Some courses in Aalto University have a variable amount of credits but most of these weren't relevant to me so in these cases I just selected the minimum amount of credits from the course as the amount.

#### Length

The length of the course in periods is represented by an unsigned integer. There were one or two courses that could be completed either in periods I-II or periods III-V so that they had variable length but for these I just manually selected the length based on when i would complete them.

#### Start and exam periods

There are two vectors of periods (see [period](#period)) modelling which periods the course starts in and which periods it has exams in. There is also a boolean varible indicating whether the course can be completed by only participating in an exam. For the most part these were also unambiguous for all Aalto University courses of interest.

#### Even or odd years

There was a non-insignificant amount of courses that were organized only every other year. For these I added an integer that takes on three values depending on whether the course is organized every year, only even years or only odd years. This is taken in to account in the course plan dialog year of studies options. Some but not all of the courses that are organized every other year still have an exam every year and this is indicated by a boolean. However, if this is the case then the exam periods depend on whether or not the course is organized that year, so the UI shows a warning text and the user should manually check (e.g. from Sisu) when the course has exams.

### Course plan

A course plan represents a plan to complete a course. They are implemented as abstract classes so they can not be instantiated. This is because every course plan has to either be a [participation plan](#participation-plan) or [exam plan](#exam-plan). A course plan contains a reference to the course that is planned, the starting period of the studies and the year of studies that the course is completed in.

#### Participation plan

A participation plan is a course plan where participation in lectures, exercise session etc. is planned. It is implemented as a class that inherits the course plan and it has no member variables besides the ones that it inherits from the course plan class.

#### Exam plan

An exam plan is a course plan where completion by only participating in an exam is planned. It also inherits the course plan class and has the length of the (self-)studies in periods, which the user can obviously decide freely, as well as the period and year of the exam. Currently the year and period of the exam aren't shown anywhere and don't affect anything. Only the start period and length of the studies is shown.

### Study plan

A study plan represents a single plan of what courses are completed and when. Functionally it is just a list of course plans but it also has a list of available courses for when a previously saved study plan is loaded and course codes need to be used to match courses to course plans.

### Period

There is an enum that inherits unsigned int for handling periods. I decided to make christmas break a period between periods II and III since it is so long and thus an excellent opportunity for self studying courses to do an exam on at the end of period III. This does cause periods III-V to be represented by the numbers 4-6 but it is a minor inconvenience. There are also two summer periods which actually do have some courses that are organized during them (as well as self-studying being an option). Some helper functions related to handling periods are implemented as well.

## File management

### Available courses

There is a single JSON file featuring an array with every course of interest to me. As an Aalto University computer science student with an interest in math these are mostly computer science and math courses in Aalto Univeristy. Each course is represented by a JSON object with exactly the same fields as the [course](#course) struct. This file is loaded by the [main](#main) function on start up and the courses are displayed in the [course selector](#course-selector). I also wrote some Python scripts for fetching courses from the Sisu API based on search criterions.

### Saving study plans

Study plans are saved as JSON files. The file is just an array of JSON objets each representing a single course plan. Objects have different fields based on whether the course plan is a participation plan or an exam plan. For each course only the course code is saved and these codes are matched with available courses when the study plan is loaded (see [study plan](#study-plan)). There is a boolean variable indicating whether the course plan is an exam plan or not and otherwise the fields are identical to the corresponding classes.