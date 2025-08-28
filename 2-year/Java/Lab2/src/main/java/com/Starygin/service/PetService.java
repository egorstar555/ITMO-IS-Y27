package com.Starygin.service;

import com.Starygin.data.entity.Pet;
import com.Starygin.data.repository.PetRepository;
import com.Starygin.service.dto.PetDto;
import com.Starygin.service.mapper.PetMapper;

import java.util.ArrayList;
import java.util.List;

public class PetService {
    private final PetRepository petRepository;

    private final PetMapper petMapper;

    public PetService(PetRepository petRepository, PetMapper petMapper) {
        this.petRepository = petRepository;
        this.petMapper = petMapper;
    }

    public List<PetDto> findAll() {
        List<PetDto> petsDto = new ArrayList<>();

        for (Pet pet: petRepository.getAll()) {
            petsDto.add(petMapper.toDto(pet));
        }

        return petsDto;
    }
}
