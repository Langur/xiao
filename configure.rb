CONFIG_FILE = 'system.i'

# 各種パラメータ定義
PARAMS = {
           'CRE_TSK': {
             'reg': /^CRE_TSK\(/, 
             'num': 7, 
             'fnum': 3, 
             'fprm': 'VP_INT extif',
             'hprm': 'const T_CRE_TSK_PRM cre_tsk_prms'
           },
           'DEF_INH': {
             'reg': /^DEF_INH\(/,
             'num': 3,
             'fnum': 2,
             'fprm': 'void',
             'hprm': 'const T_DEF_INH_PRM def_inh_prms'
           }
         }

data = { 'api': {}, 'func': {} }

PARAMS.each { |api, params|
  data[:api][api.to_s]  = []
  data[:func][api.to_s] = []
}

File.foreach(CONFIG_FILE) { |line|
  PARAMS.each { |api, params|
    if line.start_with?(api.to_s + '(') then
      ary = line.sub(params[:reg], '').gsub(/\{/, "").gsub(/\}/, "").sub(/\);/, "").gsub(/\ /, "").chomp.split(",")
      if ary.length != params[:num] then
        print(api.to_s + " format error.\r\n")
        print("  " + line.chomp + "\r\n")
        exit
      end
      data[:api][api.to_s] << ary
      data[:func][api.to_s] << ary[params[:fnum]]
    end
  }
}

# kernel_id.hの生成
INCLUDE_GUARD = "__KERNEL_ID_H__"
task_num = data[:api]['CRE_TSK'].length
inh_num = data[:api]['DEF_INH'].length
File.open("kernel_id.h", mode = "w") { |f|
  f.write("#ifndef " + INCLUDE_GUARD + "\n")
  f.write("#define " + INCLUDE_GUARD + "\n")
  f.write("#define TMAX_TSKID (" + task_num.to_s + ")\n")
  f.write("#define TMAX_CRE_TSK_PRM (" + task_num.to_s + ")\n")
  f.write("#define TMAX_DEF_INH_PRM (" + inh_num.to_s + ")\n")
  f.write("#define TPRI_IDX(pri) (pri - 1)\n")
  f.write("#define TSKID_IDX(tskid) (tskid - 1)\n")
  
  i = 0
  for task in data[:api]['CRE_TSK'] do
    f.write("#define " + task[0] + " (" + (i + 1).to_s + ")\n")
    i += 1
  end

  f.write("#endif /* " + INCLUDE_GUARD + " */\n")
}

# kernel_cfg.cの生成
File.open("kernel_cfg.c", mode = "w") { |f|
  f.write("#include <defines.h>\n")
  f.write("#include <itron.h>\n")
  f.write("#include <interrupt.h>\n")
  f.write("#include <kernel_id.h>\n")
  f.write("#include <task.h>\n")

  data[:func].each { |api, funcs|
    for func in funcs do
      f.write("extern void " + func + '(' + PARAMS[api.to_sym][:fprm] + ");\n")
    end
  }

  data[:api].each { |api, keywords|
    f.write(PARAMS[api.to_sym][:hprm] + "[] = {\n")
    for keyword in keywords do
      case api
      when 'CRE_TSK' then
        f.write("\t" + 
                "{" + 
                   keyword[0] + ", " + 
                  "{" + 
                     keyword[1] + ", " + 
                     keyword[2] + ", " + 
                     keyword[3] + ", " + 
                     keyword[4] + ", " + 
                     keyword[5] + ", " + 
                     keyword[6] + 
                  "}" +
                "},\n")
      when 'DEF_INH' then
        f.write("\t" + 
                "{" + 
                   keyword[0] + ", " + 
                  "{" + 
                     keyword[1] + ", " + 
                     keyword[2] + 
                  "}" +
                "},\n")
      else
      end
    end
    f.write("};\n")
  }
}
