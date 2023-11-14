##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=AILS_OOP
ConfigurationName      :=Release
WorkspacePath          :=C:/Users/User/Documents/AILS_OOP
ProjectPath            :=C:/Users/User/Documents/AILS_OOP/AILS_OOP
IntermediateDirectory  :=$(ConfigurationName)
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=User
Date                   :=14/11/2023
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
Preprocessors          :=$(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="AILS_OOP.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :="C:/Program Files/mingw-w64/mingw64/bin/windres.exe"
LinkOptions            :=  
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
CXXFLAGS :=  -O2 -Wall $(Preprocessors)
CFLAGS   :=  -O2 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := "C:/Program Files/mingw-w64/mingw64/bin/as.exe"


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
Objects0=$(IntermediateDirectory)/Instance.cpp$(ObjectSuffix) $(IntermediateDirectory)/Sol.cpp$(ObjectSuffix) $(IntermediateDirectory)/LocalSearchOperator.cpp$(ObjectSuffix) $(IntermediateDirectory)/AILS.cpp$(ObjectSuffix) $(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IntermediateDirectory)/heuristicsFunctions.cpp$(ObjectSuffix) $(IntermediateDirectory)/Perturbation.cpp$(ObjectSuffix) 



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
$(IntermediateDirectory)/Instance.cpp$(ObjectSuffix): Instance.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Instance.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Instance.cpp$(DependSuffix) -MM Instance.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/User/Documents/AILS_OOP/AILS_OOP/Instance.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Instance.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Instance.cpp$(PreprocessSuffix): Instance.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Instance.cpp$(PreprocessSuffix) Instance.cpp

$(IntermediateDirectory)/Sol.cpp$(ObjectSuffix): Sol.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Sol.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Sol.cpp$(DependSuffix) -MM Sol.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/User/Documents/AILS_OOP/AILS_OOP/Sol.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Sol.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Sol.cpp$(PreprocessSuffix): Sol.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Sol.cpp$(PreprocessSuffix) Sol.cpp

$(IntermediateDirectory)/LocalSearchOperator.cpp$(ObjectSuffix): LocalSearchOperator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/LocalSearchOperator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/LocalSearchOperator.cpp$(DependSuffix) -MM LocalSearchOperator.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/User/Documents/AILS_OOP/AILS_OOP/LocalSearchOperator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/LocalSearchOperator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/LocalSearchOperator.cpp$(PreprocessSuffix): LocalSearchOperator.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/LocalSearchOperator.cpp$(PreprocessSuffix) LocalSearchOperator.cpp

$(IntermediateDirectory)/AILS.cpp$(ObjectSuffix): AILS.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/AILS.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/AILS.cpp$(DependSuffix) -MM AILS.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/User/Documents/AILS_OOP/AILS_OOP/AILS.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/AILS.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/AILS.cpp$(PreprocessSuffix): AILS.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/AILS.cpp$(PreprocessSuffix) AILS.cpp

$(IntermediateDirectory)/main.cpp$(ObjectSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/main.cpp$(DependSuffix) -MM main.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/User/Documents/AILS_OOP/AILS_OOP/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.cpp$(PreprocessSuffix): main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.cpp$(PreprocessSuffix) main.cpp

$(IntermediateDirectory)/heuristicsFunctions.cpp$(ObjectSuffix): heuristicsFunctions.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/heuristicsFunctions.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/heuristicsFunctions.cpp$(DependSuffix) -MM heuristicsFunctions.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/User/Documents/AILS_OOP/AILS_OOP/heuristicsFunctions.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/heuristicsFunctions.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/heuristicsFunctions.cpp$(PreprocessSuffix): heuristicsFunctions.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/heuristicsFunctions.cpp$(PreprocessSuffix) heuristicsFunctions.cpp

$(IntermediateDirectory)/Perturbation.cpp$(ObjectSuffix): Perturbation.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/Perturbation.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/Perturbation.cpp$(DependSuffix) -MM Perturbation.cpp
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/User/Documents/AILS_OOP/AILS_OOP/Perturbation.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/Perturbation.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/Perturbation.cpp$(PreprocessSuffix): Perturbation.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/Perturbation.cpp$(PreprocessSuffix) Perturbation.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r $(ConfigurationName)/


