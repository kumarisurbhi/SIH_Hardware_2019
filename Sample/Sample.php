<?php

function func_name(){
################ Global Variables used in code ################
$file = "Sample.c";
$auto_eval_path = "../../";
$path = "./";

$result=[];
$Marks = 0;
$Remark = "";
$main_flag = 0;

################# Check whether user file is present ################
if(file_exists($file))
{
    ############### Check file is not empty ################
    if (filesize($file))
    {
        ############## Create temporary header file and c file ################
        $file_write = $path."/Temp.c"; 
        
        $handle = fopen($file_write, "w");

        ############## Create array of functions to provide appropriate remarks ################
        $funtions_to_check = array("buzzer_on()","buzzer_off()");
        $test_functions = array("test_buzzer_on","test_buzzer_off");

        ############## Change the temp.c file according to the requirement ################
        $arr = file($file);
        foreach($arr as $line)
        {
            if (strpos($line, 'lpc214x.h'))
                fwrite($handle, "#include \"".$auto_eval_path."utilities/ARM_Header_Files/LPC214x.h\"\n");
            else
            {
                if(strpos($line, 'main()') || strpos($line, 'main(void)'))
                {
                    $main_flag = 1;
                    fwrite($handle, "#ifndef TEST\n");
                }
                fwrite($handle, $line);
            }
        }

        if($main_flag == 1)
            fwrite($handle, "\n#endif");
        
        ############## Run Test.c file ################
        exec("gcc -Os -DTEST -std=c99 ".$file_write." ".$auto_eval_path."Scripts/Sample/Sample_test.c ".$auto_eval_path."utilities/CMock/lib/mocks/Mock_ms_delay.c ".$auto_eval_path."utilities/Unity-master/src/unity.c -o ".$path."Test 2>&1",$code_exec);
        // Log::info($code_exec);
        var_dump($code_exec);
        ############### Check if test.c ran successfully ################
        if (file_exists($path."Test"))
        {
            exec("ulimit -t 1; ".$path."Test 2>&1",$output);
            // Log::info($output);
            var_dump($output);
            ############### If code consist of infinite loop ################
            if ($output[0]=="Killed")
            {
               $Marks = 0;
               $Remark = "Execution time is High\n"; 
            }
            ############### If Test result is Fail ################
            if (strcmp($output[sizeof($output)-1], "FAIL") == 0)
            {
                $Marks = 0;
                $Remark = "Logical Error <br>";
            }

            ############### If Test result is Pass ################
            if (strcmp($output[sizeof($output)-1], "OK") == 0)
            {
                $Marks = 10;
                $Remark = "Congrats \n";
            }

            ############### Provide Function wise Remark ################
            for ($i=0;$i<count($output);$i++)
            {
                if (strpos($output[$i],'FAIL'))
                {
                    $Marks = 0;
                    for ($j=0; $j<count($test_functions); $j++)
                        if(strpos($output[$i],$test_functions[$j]))
                            $Remark .= "Error in Function ".$funtions_to_check[$j]."<br>";
                }
            }
        }

        ############## test.c failed ################
        else
        {
                $Marks = 0;
                $Remark = "Error in .c File. Functions name might be changed.";
        }

        ############## Check if user.c ran successfully ################
        exec("gcc -Os -std=c99 ".$file_write. " -o ".$path. "Test_c 2>&1",$user_code);
        var_dump($user_code);
        // Log::info($user_code);
        for ($i=0;$i<count($user_code);$i++)
            if (strpos($user_code[$i],'error'))
            {
                $Marks = 0;
                $Remark = "Error in .c File";
            }

        ############## delete the created files ################
       //unlink($file_write);
       //unlink($file_write_header);
        if (file_exists($path."Test"))    
            unlink($path."Test");
        if (file_exists($path."Test_c"))    
            unlink($path."Test_c");
    }

    else
    {
        $Marks = 0;
        $Remark = "Empty File";
    }
}

else
{
    $Marks = 0;
    $Remark = "File doesnot exist";
}
############## Memory access error ################
if($Remark == "")
    $Remark = "Error in file";

echo "\nMarks Scored = ".$Marks."\n\n";
echo "Remarks = ".$Remark."\n\n";

$result["Marks"]=$Marks;
$result["Remarks"]=$Remark;
return $result;

}

func_name()

?>