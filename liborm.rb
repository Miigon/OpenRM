def __OpenRM_debug(x = "")
  print x.to_s() + "\n"
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
