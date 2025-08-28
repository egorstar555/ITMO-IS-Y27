package com.Starygin.service;

import com.Starygin.data.entity.Owner;
import com.Starygin.data.repository.OwnerRepository;
import com.Starygin.service.dto.OwnerDto;
import com.Starygin.service.mapper.OwnerMapper;

import java.util.ArrayList;
import java.util.List;

public class OwnerService {
    private final OwnerRepository ownerRepository;

    private final OwnerMapper ownerMapper;

    public OwnerService(OwnerRepository ownerRepository, OwnerMapper ownerMapper) {
        this.ownerRepository = ownerRepository;
        this.ownerMapper = ownerMapper;
    }

    public List<OwnerDto> findAll() {
        List<OwnerDto> ownerDto = new ArrayList<>();

        for (Owner owner: ownerRepository.getAll()) {
            ownerDto.add(ownerMapper.toDto(owner));
        }

        return ownerDto;
    }
}
