def __OpenRM_debug(x = "")
  print x.to_s() + "\n" #TEMP#
end

def load_data(filename) # Load rvdata file
  File.open(filename, "rb") { |f|
    return Marshal.load(f)
  }
  # TODO: Encrypted data support
end

# RPG Maker use zlib to compress scripts
require "zlib"
include Zlib

def __OpenRM_LoadGameProject(path)
  #return ;
  __OpenRM_ChangeDirectory path
  __OpenRM_debug "Loading game project from: " + path
  # TODO: Get script path in `game.ini`
  scripts = load_data(path + "/Data/Scripts.rvdata") #TEMP#
  scripts.each{|s|
    # TODO: What is `s[0]`?
    decompressed_script = Inflate.inflate(s[2])
    if decompressed_script.length != 0
      __OpenRM_debug "Load script: " + s[1]
      eval(decompressed_script,nil,s[1],1)
    end
  }
end

def TableTestScript
tbl = Table.new(12,21,10)
tbl[11,20,9] = 88
__OpenRM_debug tbl[11,20,9]
tbl.resize(15,28,20)
__OpenRM_debug tbl[11,20,9]
__OpenRM_debug tbl[12,21,10]
__OpenRM_debug tbl[14,27,19]
tbl[14,27,19] = 621
__OpenRM_debug tbl[14,27,19]
end
#TableTestScript()
