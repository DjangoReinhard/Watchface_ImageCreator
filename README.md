# Watchface_ImageCreator
linux application to assist in creation of all the needed images for a watchface

![Preview](sample/WFCreator01.png)

## usage
- to start, select "**new**" from main menu and select an (empty) directory
- use the **dropdown combobox** to select the element you want to edit
- change **x** and **y** coordinates to position your element
- using **font** and **color** buttons you can style your element
- when you're done, select "**save**" from main menu and select a directory where you want to put your images

- when reopening a project, select "**open**" from main menu and select the "*watchface-pre.txt*" file written at last save.

## build
- this project uses qt framework with qmake as build system
- either use QtCreator or build it manually like this:

        git clone https://github.com/DjangoReinhard/Watchface_ImageCreator.git
        cd Watchface_ImageCreator
        mkdir build
        cd build
        qmake ../FaceBuilder.pro
        make

### execute
- dive into the build directory and execute:

        ./FaceBuilder         

## localization
- english is default language

- see i18n/FaceBuilder_*de_DE*.ts
- copy that file and rename "*de_DE*" part to match your language
- use *Qt Linguist* to translate and *lrelease* to create the qm-file
- rebuild project
