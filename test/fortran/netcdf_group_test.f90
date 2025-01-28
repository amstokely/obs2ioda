

subroutine add_netcdf_group_test()
    use test_utils_mod
    use netcdf_cxx_mod
    use iso_c_binding, only: c_int, c_null_char
    implicit none
    integer :: status
    integer(c_int) :: netcdfID
    character(len=:), allocatable :: path
    integer(c_int) :: fileMode
    integer(c_int) :: result

    path = "test_group.nc"
    fileMode = 2
    result = netcdfCreate(path, netcdfID, fileMode)
    call assertEqual(0, result, status, assert)
    result = netcdfAddGroup(netcdfID, "group1")
    call assertEqual(0, result, status, assert)
    result = netcdfAddGroup(netcdfID, "group2", parentGroupName="group1")
    call assertEqual(0, result, status, assert)
    result = netcdfAddGroup(netcdfID, "group3", parentGroupName=c_null_char)
    call assertEqual(-1, result, status, assert)
    result = netcdfClose(netcdfID)
    call assertEqual(0, result, status, assert)

end subroutine add_netcdf_group_test

program netcdf_group_test

    call add_netcdf_group_test()

end program netcdf_group_test