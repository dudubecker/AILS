##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=AILS_OOP
ConfigurationName      :=Debug
WorkspacePath          :=C:/Users/User/Documents/AILS_OOP
ProjectPath            :=C:/Users/User/Documents/AILS_OOP/AILS_OOP
IntermediateDirectory  :=$(ConfigurationName)
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=User
Date                   :=26/12/2022
CodeLitePath           :="C:/Program Files/CodeLite"
LinkerName             :="C:/Program Files/mingw-w64/mingw64/bin/g++.exe"
SharedObjectLinkerName :="C:/Program Files/mingw-w64/mingw64/bin/g++.exe" -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="AILS_OOP.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :="C:/Program Files/mingw-w64/mingw64/bin/windres.exe"
LinkOptions            :=  -static
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := "C:/Program Files/mingw-w64/mingw64/bin/ar.exe" rcu
CXX      := "C:/Program Files/mingw-w64/mingw64/bin/g++.exe"
CC       := "C:/Program Files/mingw-w64/mingw64/bin/gcc.exe"
CXXFLAGS :=  -g -O0 -std=c++17 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := "C:/Program Files/mingw-w64/mingw64/bin/as.exe"


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
Objects0=$(IntermediateDirectory)/Heuristic.cpp$(ObjectSuffix) $(IntermediateDirectory)/Sol.cpp$(ObjectSuffix) $(IntermediateDirectory)/Instance.cpp$(ObjectSuffix) $(IntermediateDirectory)/main.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@$(MakeDirCommand) "$(ConfigurationName)"


$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "$(ConfigurationName)"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/Heuristic.cpp$(ObjectSuffix): Heuristic.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Heuristic.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Heuristic.cpp$(DependSuffix) -MM Heuristic.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/User/Documents/AILS_OOP/AILS_OOP/Heuristic.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Heuristic.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Heuristic.cpp$(PreprocessSuffix): Heuristic.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Heuristic.cpp$(PreprocessSuffix) Heuristic.cpp

$(IntermediateDirectory)/Sol.cpp$(ObjectSuffix): Sol.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Sol.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Sol.cpp$(DependSuffix) -MM Sol.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/User/Documents/AILS_OOP/AILS_OOP/Sol.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Sol.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Sol.cpp$(PreprocessSuffix): Sol.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Sol.cpp$(PreprocessSuffix) Sol.cpp

$(IntermediateDirectory)/Instance.cpp$(ObjectSuffix): Instance.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Instance.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Instance.cpp$(DependSuffix) -MM Instance.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/User/Documents/AILS_OOP/AILS_OOP/Instance.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Instance.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Instance.cpp$(PreprocessSuffix): Instance.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Instance.cpp$(PreprocessSuffix) Instance.cpp

$(IntermediateDirectory)/main.cpp$(ObjectSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/main.cpp$(DependSuffix) -MM main.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/User/Documents/AILS_OOP/AILS_OOP/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.cpp$(PreprocessSuffix): main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.cpp$(PreprocessSuffix) main.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(ConfigurationName)/


