# This is a build script for STM32F429 Discovery ARM board
#
# References:
# [1] "STM32 ST-LINK Utility software description", User manual UM0892, DocID16987 Rev 19, July 2015,
#     ARM Ltd., (/assets/pdf/CD00262073.pdf)

# Load cbuild script to help build C program
load File.join(File.dirname(__FILE__), 'cbuild.rb')

#flasher = "/C/Program Files (x86)/STMicroelectronics/STM32 ST-LINK Utility/ST-LINK Utility/ST-LINK_CLI"
flasher = get_value_from_env("flasher", "ST-LINK_CLI")
elf_to_hex = get_value_from_env("elf_to_hex", "arm-none-eabi-objcopy")

# ST-LINK_CLI error code. They can be found in [1].
ERR_ST_LINK_ARGS = 1
ERR_ST_LINK_CONNECTION = 2
ERR_ST_LINK_COMMAND = 3
ERR_ST_LINK_RESET = 7
ERR_ST_LINK_HALT = 8
ERR_ST_LINK_STEP = 9
ERR_ST_LINK_BREAKPOINT = 10
ERR_ST_LINK_ERASE_FLASH = 11
ERR_ST_LINK_PROG_VERIFY = 12

# Configuration parameters
config = {
  :verbose      => :no,
  :compiler     => 'arm-none-eabi-gcc',
  :linker       => 'arm-none-eabi-gcc',
# -IC:\Users\user26\CoIDE\workspace\RTOS
  :include_path => ['Drivers/CMSIS/Include',
                    'Drivers/STM32F4xx_HAL_Driver/Inc',
                    'app/Config',
                    'app'],
  :user_define  => ['STM32F429ZI', 'STM32F429xx', 'STM32F429_439xx', '__FPU_USED'],
  :library_path => '.',
#  :library => ['libusb'],
  :linker_script => 'stm32f429zi_flash.ld',
#  :compiler_options => ['-mcpu=cortex-m4 -mthumb -g2 -Wall -O0 -g'],          # Other compiler options
  :compiler_options => ['-mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -g2 -Wall -O0 -g'],          # Other compiler options
#  :linker_options => ['-mcpu=cortex-m4 -mthumb -g2 -O0 -Wl,--gc-sections'],   # Other linker options
  :linker_options => ['-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -g2 -O0 -Wl,--gc-sections'],   # Other linker options
  :option_keys  => {:library => '-l',
                    :library_path => '-L',
                    :include_path => '-I',
                    :output_file => '-o',
                    :compile => '-c',
                    :linker_script => '-Wl,-T',
                    :define => '-D'}
}


namespace :hw do
  OUTPUT_PATH = 'build/release/hw/'
  ouput_elf = nil
  ouput_hex = nil
  task :prepare_release, [:coproj] do |t, args|
    filenames, coproj = get_all_source_files_in_coproj(args[:coproj])
    puts "Building sources in #{coproj}..."
    file = File.basename(coproj, '.*')
    ouput_elf = File.join(OUTPUT_PATH, file + '.elf')
    ouput_hex = File.join(OUTPUT_PATH, file + '.hex')
    dep_list = createCompilationDependencyList(filenames, ['c', '.c++', '.s', 'cpp', 'asm'], '.', '.o')
    dep_list = compile_list(dep_list, '.', OUTPUT_PATH, '.', config)
  #  p dep_list
    link_all(getDependers(dep_list), ouput_elf, config)

    file ouput_hex => ouput_elf do |n|
      elf_to_hex = ensure_program_available(elf_to_hex,                       \
              "Error: Cannot find #{elf_to_hex} program to turn ELF to HEX.")
      if File.exists? n.prerequisites[0]
        puts "converting #{n.prerequisites[0]} to hex..."
        sys_cli "#{elf_to_hex} -O ihex #{n.prerequisites[0]} #{n.name}"
      else
        raise ArgumentError,                                                  \
        "Error: Can't convert to #{n.name} because can't find #{n.prerequisites[0]}."
      end
    end
  end
#  CLEAN.include('build/release/hw') if File.exist? 'build/release/hw'

  desc 'Build hardware release code'
  task :release, [:coproj] => :prepare_release do |t, args|
  #  p Rake.application.tasks

    Rake::Task[ouput_elf].invoke(args)
  #  sh "cp #{OUTPUT_FILE} ."
  end

  desc 'Flash program and run test'
  task :flash, [:coproj] => :prepare_release do |t, args|
    Rake::Task[ouput_hex].invoke(args)
    flasher = ensure_program_available(flasher,                             \
              "Error: Cannot find #{flasher} program to flash ARM processor.")
    # First check if there is any differences between current Hex file with
    # the one on the MCU Flash. Download if there is, otherwise do nothing.
    if (!system "#{flasher} -CmpFile #{ouput_hex}") && ($?.exitstatus != ERR_ST_LINK_CONNECTION)
      # Flash the Hex file into the MCU Flash
      sys_cli "#{flasher} -P #{ouput_hex} -V while_programming -Rst -Run" \
                                                  if File.exists? ouput_hex
    end
  end

  desc 'Erase all Flash sectors'
  task :full_erase do
    flasher = ensure_program_available(flasher,                             \
              "Error: Cannot find #{flasher} program to full chip erase.")
    sys_cli "#{flasher} -ME"
  end

  desc "Just duplicating .gitignore"
  task :ignore do
    src = ".gitignore"
    target = ".gitignoreXXX"
    if !up_to_date?(target, src)
      p "duplicating .gitignore"
      sh "cp #{src} #{target}"
    else
      p "already have the latest copy..."
    end
  end
end
