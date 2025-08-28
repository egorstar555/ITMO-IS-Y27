package com.Starygin.view;

import com.Starygin.service.dto.UserDto;
import com.Starygin.service.UserService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.*;

@RestController
@RequestMapping("/api/users")
public class UserController {

    private final UserService userService;

    @Autowired
    public UserController(UserService userService) {
        this.userService = userService;
    }

    @PostMapping
    @ResponseStatus(HttpStatus.CREATED)
    public UserDto CreateUser(@RequestBody UserDto userDto) {
        return userService.create(userDto);
    }
}
