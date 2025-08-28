package com.Starygin.service;

import com.Starygin.service.dto.UserDto;
import com.Starygin.model.entity.User;
import com.Starygin.model.repository.UserRepository;
import com.Starygin.service.mapper.UserMapper;
import com.Starygin.service.security.SecurityConfig;
import com.Starygin.service.security.UserDetailsImpl;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.core.userdetails.UsernameNotFoundException;
import org.springframework.stereotype.Service;

@Service
public class UserService implements UserDetailsService {
    private final UserRepository userRepository;

    private final UserMapper userMapper;

    @Autowired
    public UserService(UserRepository userRepository, UserMapper userMapper) {
        this.userRepository = userRepository;
        this.userMapper = userMapper;
    }

    public UserDto create(UserDto userDto) {
        var userToSave = userMapper.toEntity(userDto);
        userToSave.setPassword(SecurityConfig.passwordEncoder().encode(userDto.password()));

        return userMapper.toDto(userRepository.save(userToSave));
    }

    @Override
    public UserDetails loadUserByUsername(String username) throws UsernameNotFoundException {
        User user = userRepository.findByName(username);
        if (user == null) {
            throw new UsernameNotFoundException(username);
        }

        return new UserDetailsImpl(user);
    }
}
