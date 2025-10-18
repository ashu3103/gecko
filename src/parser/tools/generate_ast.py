import os
import json
import io

HEADER_EXTENSION = ".h"
FILE_EXTENSION = ".cpp"

BUFFER_SIZE = 100 * 1024

INDENT = 0
OUT = ""

## Libraries to include
LIBRARIES_TO_INCLUDE = [
    'iostream',
    'variant',
    'token.h'
]

SCRIPT_DIRECTORY = os.getcwd()
PARSER_DIRECTORY = os.path.dirname(SCRIPT_DIRECTORY)

def AppendOrFlush(file: io.TextIOWrapper, stmts: str):
    global OUT
    global BUFFER_SIZE

    if (len(OUT) + len(stmts) >= BUFFER_SIZE):
        file.write(OUT)
        OUT = stmts
    else:
        OUT = OUT + stmts

## write directive start
def WriteDirectiveStart(file: io.TextIOWrapper, directive: str):
    st = ''
    st = st + Line(f'#ifndef {directive}')
    st = st + Line(f'#define {directive}')
    AppendOrFlush(file, st)

## write directive end
def WriteDirectiveEnd(file: io.TextIOWrapper, directive: str):
    st = ''
    st = st + Line(f'#endif //{directive}')
    AppendOrFlush(file, st)

## write newline characters
def Newline() -> str:
    return '\n'

## write indent
def Indentation() -> str:
    global INDENT
    st = ""
    for i in range(INDENT):
        st = st + '\t'
    return st

def Line(line: str) -> str:
    return f'{Indentation()}{line}{Newline()}'

## write libraries included
def WriteIncludeLibraries(file: io.TextIOWrapper):
    st = ''
    for lib in LIBRARIES_TO_INCLUDE:
        st = st + Line(f'#include <{lib}>')
    AppendOrFlush(file, st)

## write the forward declaration of derived classes
def WriteForwardDeclaration(file: io.TextIOWrapper, classes: list):
    st = ''
    for i, cls in enumerate(classes):
        cls_name = cls['name']
        st = st + Line(f'struct {cls_name};')
    AppendOrFlush(file, st)

## write variant
def WriteVariant(file: io.TextIOWrapper, basename: str, classes: list):
    global INDENT

    st = ''
    derived = ''
    for i, cls in enumerate(classes):
        cls_name = cls['name']
        if i == 0:
            derived = derived + f'{cls_name}*'
        else:
            derived = derived + f', {cls_name}*'

    st = st + Line(f'using {basename} = std::variant<{derived}>;')

    AppendOrFlush(file, st)

## write the base class
# def WriteBaseClass(file: io.TextIOWrapper, basename: str, classes: list):
#     global INDENT

#     st = ''
#     st = st + Line(f'template <typename R>')
#     st = st + Line(f'class {basename} {{')
#     INDENT = INDENT + 1

#     st = st + Line(f'public:')
#     INDENT = INDENT + 1

#     st = st + Line(f'virtual ~{basename}() = default;')
#     st = st + Line(f'virtual R accept(Visitor<R>& visitor) = 0;')

#     INDENT = INDENT - 2
#     st = st + Line(f'}};')

#     AppendOrFlush(file, st)

## write the data fields
def WriteDataFields(file: io.TextIOWrapper, fields: list):
    st = ''
    for i, field in enumerate(fields):
        st = st + Line(f'{field['type']} {field['name']};')
    st = st + Newline()
    AppendOrFlush(file, st)

## write the constructor
def WriteDerivedClassConstructor(file: io.TextIOWrapper, cls_name: str, fields: list):
    num_of_fields = len(fields)
    params = ''
    st = ''
    member_init_list = ''
    for i, field in enumerate(fields):
        if (i == 0):
            params = params + field['type'] + ' ' + field['name']
            member_init_list = member_init_list + field['name'] + "(" + field['name'] + ")"
        else:
            params = params + ", " + field['type'] + ' ' + field['name']
            member_init_list = member_init_list + ', ' + field['name'] + "(" + field['name'] + ")"
    
    if (num_of_fields):
        st = st + Line(f'{cls_name}({params}): {member_init_list} {{}}')
    else:
        st = st + Line(f'{cls_name}({params}) {{}}')
    AppendOrFlush(file, st)

## write the accept implementation
def WriteAccept(file: io.TextIOWrapper, basename, cls_name):
    global INDENT
    st = ''
    st = st + Line(f'R accept(Visitor<R>& visitor) override {{')
    INDENT = INDENT + 1
    st = st + Line(f'return visitor.visit{cls_name}{basename}(this);')
    INDENT = INDENT - 1
    st = st + Line(f'}}')
    AppendOrFlush(file, st)

## write the derived class
def WriteDerivedClass(file: io.TextIOWrapper, basename: str, cls: dict):
    global INDENT
    cls_name = cls['name']
    fields = cls['fields']

    AppendOrFlush(file, Line(f'struct {cls_name} {{'))
    INDENT = INDENT + 1
    ## data fields
    WriteDataFields(file, fields)
    ## constructor
    WriteDerivedClassConstructor(file, cls_name, fields)

    INDENT = INDENT - 1
    AppendOrFlush(file, Line(f'}};'))
    
## write the header file
def WriteHeaderFile(file: io.TextIOWrapper, data: dict):
    global INDENT

    basename = data['basename']
    directive = data['directive']
    namespace = data['namespace']
    classes = data['classes']

    WriteDirectiveStart(file, directive)
    ## Include Libraries
    AppendOrFlush(file, Newline())
    WriteIncludeLibraries(file)

    AppendOrFlush(file, Newline())

    ## Create the namespace
    AppendOrFlush(file, Line(f'namespace {namespace} {{'))
    INDENT = INDENT + 1

    ## Create the forward declaration
    WriteForwardDeclaration(file, classes)
    AppendOrFlush(file, Newline())

    WriteVariant(file, basename, classes)
    ## Create the base class
    # WriteBaseClass(file, basename, classes)
    AppendOrFlush(file, Newline())

    ## Create Derived Classes
    for i, cls in enumerate(classes):
        WriteDerivedClass(file, basename, cls)
        AppendOrFlush(file, Newline())

    INDENT = INDENT - 1
    AppendOrFlush(file, Line(f'}}'))
    AppendOrFlush(file, Newline())
    WriteDirectiveEnd(file, directive)

## write derived class constructor definition
# def WriteDerivedClassConstructorDef(file: io.TextIOWrapper, cls_name: str, fields: list):
#     params = ''
#     st = ''
#     member_init_list = ''
#     for i, field in enumerate(fields):
#         if (i == 0):
#             params = params + field['type'] + ' ' + field['name']
#             member_init_list = member_init_list + field['name'] + "(" + field['name'] + ")"
#         else:
#             params = params + ", " + field['type'] + ' ' + field['name']
#             member_init_list = member_init_list + ', ' + field['name'] + "(" + field['name'] + ")"
    
#     st = st + Line(f'template <typename R>')
#     st = st + Line(f'{cls_name}<R>::{cls_name}({params}): {member_init_list} {{')
#     st = st + Line(f'}}')
#     st = st + Newline()

#     AppendOrFlush(file, st)

## write the accept definition
# def WriteAcceptDef(file: io.TextIOWrapper, basename: str, cls_name: str):
#     global INDENT
#     st = ''
#     st = st + Line(f'template <typename R>')
#     st = st + Line(f'R {cls_name}<R>::accept(Visitor<R>& visitor) {{')
#     INDENT = INDENT + 1
#     st = st + Line(f'return visitor.visit{cls_name}{basename}(this);')
#     INDENT = INDENT - 1
#     st = st + Line(f'}}')
#     AppendOrFlush(file, st)

## write definitions
# def WriteDefinition(file: io.TextIOWrapper, header: str,  data: dict):
#     global INDENT

#     namespace = data['namespace']
#     basename = data['basename']
#     classes = data['classes']
#     ## Include the header file
#     AppendOrFlush(file, Line(f'#include <{header}>'))
#     AppendOrFlush(file, Newline())
#     ## Create the namespace
#     AppendOrFlush(file, Line(f'namespace {namespace} {{'))
#     INDENT = INDENT + 1

#     for i, cls in enumerate(classes):
#         cls_name = cls['name']
#         fields = cls['fields']
#         ## Constructor Definition
#         # WriteDerivedClassConstructorDef(file, cls_name, fields)
#         WriteAcceptDef(file, basename, cls_name)
#         AppendOrFlush(file, Newline())
    
#     INDENT = INDENT - 1
#     AppendOrFlush(file, Line(f'}}'))

## define an abstraction of AST
def DefineAST(data: dict):
    global PARSER_DIRECTORY
    global OUT

    # Extract values
    filename = data['filename']
    dir = PARSER_DIRECTORY

    file = os.path.join(dir, filename)
    header_file = file + HEADER_EXTENSION
    definition_file = file + FILE_EXTENSION
    ## Write Header files
    try:
        OUT = ''
        with open(header_file, 'w+') as hf:
            WriteHeaderFile(hf, data)
            hf.write(OUT)

        # OUT = ''
        # with open(definition_file, 'w+') as df:
        #     WriteDefinition(df, filename + HEADER_EXTENSION, data)
        #     df.write(OUT)
    except Exception as e:
        print(f'Error while defining AST for {filename}: {e}')

## Validate the json schema
def ValidateJSONFormat(data, filename: str):
    ## Level 1: top level
    if not isinstance(data, dict):
        raise TypeError(f"Root of {filename} must be a JSON object")
    
    # Required top level fields
    required_top_fields = ["directive", "namespace", "filename", "basename", "classes"]
    for field in required_top_fields:
        if field not in data:
            raise ValueError(f"Missing top-level field '{field}' in {filename}")

    # directive, basename, namespace, filename, must be a str
    if not isinstance(data["directive"], str):
        raise TypeError(f"'directive' must be a string in {filename}")
    
    if not isinstance(data["namespace"], str):
        raise TypeError(f"'namespace' must be a string in {filename}")
    
    if not isinstance(data["filename"], str):
        raise TypeError(f"'filename' must be a string in {filename}")
    
    if not isinstance(data["basename"], str):
        raise TypeError(f"'basename' must be a string in {filename}")

    # classes must be a list
    if not isinstance(data["classes"], list):
        raise TypeError(f"'classes' must be a list in {filename}")    
    
    ## Level 2: inside each class
    for i, cls in enumerate(data["classes"]):
        if not isinstance(cls, dict):
            raise TypeError(f"Class entry #{i} in {filename} must be an object")

        if "name" not in cls or "fields" not in cls:
            raise ValueError(f"Each class in {filename} must have 'name' and 'fields' keys")

        if not isinstance(cls["name"], str):
            raise TypeError(f"'name' of class #{i} in {filename} must be a string")

        if not isinstance(cls["fields"], list):
            raise TypeError(f"'fields' of class '{cls['name']}' in {filename} must be a list")
        
        # Level 3: inside each field object
        for j, field in enumerate(cls["fields"]):
            if not isinstance(field, dict):
                raise TypeError(f"Field #{j} in class '{cls['name']}' must be an object")

            required_field_keys = ["type", "name"]
            for key in required_field_keys:
                if key not in field:
                    raise ValueError(f"Field #{j} in class '{cls['name']}' missing '{key}' key in {filename}")

                if not isinstance(field[key], str):
                    raise TypeError(
                        f"'{key}' in field #{j} of class '{cls['name']}' must be a string in {filename}"
                    )

if __name__ == "__main__":
    ## Scan all the json file in the current directory
    for filename in os.listdir(SCRIPT_DIRECTORY):
        if filename.endswith(".json"):
            file_path = os.path.join(SCRIPT_DIRECTORY, filename)
            try:
                with open(file_path, 'r') as f:
                    data = json.load(f)
                    ## validate the json schema
                    ValidateJSONFormat(data, file_path)
                    ## define AST abstract classes
                    DefineAST(data)
            except json.JSONDecodeError as e:
                print(f"Error decoding JSON in {filename}: {e}")
            except FileNotFoundError:
                print(f"File not found: {filename}")
            except (ValueError, TypeError) as e:
                print(f"Schema validation failed for {filename}: {e}")
            except Exception as e:
                print(f"Unexpected error in {filename}: {e}")